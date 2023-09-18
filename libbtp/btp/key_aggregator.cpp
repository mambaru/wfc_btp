#include "key_aggregator.hpp"
#include "types.hpp"
#include <wrtstat/aggregator/aggregator.hpp>
#include <functional>

namespace wamba{ namespace btp{

key_aggregator::key_aggregator(mutex_type& m, key_id_t id, time_type ts, const options_type& opt)
  : _mutex(m)
  , _aggregator(ts, opt)
  , _aggregate_last_point(opt.aggregate_last_point)
{
  _key_info.id = id;
}

/*
key_aggregator::key_aggregator(key_aggregator&& other)
  : _mutex(other._mutex)
  , _key_info( other._key_info)
  , _aggregator( std::move(other._aggregator) )
  , _aggregate_last_point(other._aggregate_last_point)
{}

key_aggregator& key_aggregator::operator=(key_aggregator&& other)
{
  _mutex = other._mutex;
  _aggregator = std::move(other._aggregator);
  _aggregate_last_point = other._aggregate_last_point;
  _key_info = other._key_info;
  return *this;
}
*/

bool key_aggregator::merge(aggregated_list&& data, std::vector<aggregated_info>* up_data)
{
  for ( aggregated_info& d : data )
  {
    d.ts = _aggregator.get_separator().get_ts(d.ts);
    this->update_info_( d );
    if (up_data!=nullptr)
    {
      if ( up_data->empty() || up_data->back().ts != d.ts )
        up_data->push_back( d );
      else
        up_data->back() += d;
    }
  }
  return true;
}
  
bool key_aggregator::add(aggregated_data&& data, std::vector<aggregated_info>* up_data, bool* is_last)
{
  std::lock_guard<mutex_type> lk(_mutex);
  using namespace std::placeholders;
  time_type current = _aggregator.get_separator().current_time();
  // current выровнен в соответствии с шагом аггрегации 
  if ( data.ts != 0 &&  data.ts < current )
  {
    // Если данные идут с запаздыванием, то мы просто передаем их для сохранения
    // Т.к они идут мимо агрегатора, то считаем перцентили на том что есть
    wrtstat::basic_aggregator::calc_perc(data);

    typedef wrtstat::reduced_info reduced_info;
    typedef wrtstat::aggregated_perc aggregated_perc;
    aggregated_info ai;
    static_cast<reduced_info&>( ai ) = static_cast<const reduced_info&>(data);
    static_cast<aggregated_perc&>( ai ) = static_cast<const aggregated_perc&>(data);
    static_cast<reduced_info&>(ai).ts = _aggregator.get_separator().get_ts(data.ts);


    if ( up_data!=nullptr )
      up_data->push_back(ai);
    *is_last = false;
    return true;
  }
    
  *is_last = true;
  // Аггрегация последней точки
  // Сюда доходим если последняя точка обновляеться порциями
  // Например за текущий час, данные приходят каждую минуту, 
  // а не сразу полностью аггрегированные за час
    
  if ( data.data.empty() && data.max!=0 )
  {
    // Если набор значенией пустой, но перцентели не пустые
    // то генерируем для них данные, чтобы не усреднять последнюю точку.
    data.data.reserve(100);
    for (int i = 0 ; i < 51; ++i)
      data.data.push_back(data.perc50);
    for (int i = 0 ; i < 30; ++i)
      data.data.push_back(data.perc80);
    for (int i = 0 ; i < 15; ++i)
      data.data.push_back(data.perc95);
    for (int i = 0 ; i < 4; ++i)
      data.data.push_back(data.perc99);
    data.data.push_back(data.perc100);
  }
    
  auto handler1 = std::bind(&key_aggregator::add_handler_, this, up_data, _1);
  _aggregator.push(data, handler1);
  
  this->aggregate_last_point_if_(up_data, false);
  // аггрегация текущей (последней точки), когда недостаточно данных для финальной аггрегации
  return true;
}

void key_aggregator::aggregate_last_point_if(std::vector<aggregated_info>* up_data, bool force)
{
  std::lock_guard<mutex_type> lk(_mutex);
  this->aggregate_last_point_if_(up_data, force);
}

  
void key_aggregator::aggregate_last_point_if_(std::vector<aggregated_info>* up_data, bool force)
{
  if ( _aggregate_last_point )
  {
    // если данные поступают с интервалом >= шагу аггрегации
    // то это опция бесполезная и даже вредная, т.к. каждая точка будет
    // возвращатся два раза, сейчас и при следующем добавлении.
    aggregated_data::ptr ag;
    if ( force )
      ag = _aggregator.force_pop();
    else
      ag = _aggregator.aggregate_current();

    if ( ag != nullptr )
    {
      typedef wrtstat::reduced_info reduced_info;
      typedef wrtstat::aggregated_perc aggregated_perc;

      aggregated_info ai;
      static_cast<reduced_info&>( ai ) = static_cast<reduced_info&>(*ag);
      static_cast<aggregated_perc&>( ai ) = static_cast<aggregated_perc&>(*ag);
      this->update_info_(ai);
      if ( up_data!=nullptr )
        up_data->push_back( ai );
    }
  }
}

stored_key key_aggregator::get_key_info() const
{
  std::lock_guard<mutex_type> lk(_mutex);
  return _key_info;
}
  
void key_aggregator::set_key_info(const stored_values& ki)
{
  std::lock_guard<mutex_type> lk(_mutex);
  time_type beg_ts = ki.last_update; //?? =_aggregator.get_separator().current_time();
  //! ts -= интевал для 3000 точек
  this->set_field_(beg_ts, _key_info.count,  ki.count); 
  this->set_field_(beg_ts, _key_info.lossy,  ki.lossy);

  this->set_field_(beg_ts, _key_info.max.min,    ki.max.min);
  this->set_field_(beg_ts, _key_info.max.perc50, ki.max.perc50);
  this->set_field_(beg_ts, _key_info.max.perc80, ki.max.perc80);
  this->set_field_(beg_ts, _key_info.max.perc95, ki.max.perc95);
  this->set_field_(beg_ts, _key_info.max.perc99, ki.max.perc99);
  this->set_field_(beg_ts, _key_info.max.perc100,ki.max.perc100);
  this->set_field_(beg_ts, _key_info.max.max,    ki.max.max);
  this->set_field_(beg_ts, _key_info.max.avg,    ki.max.avg);

  this->set_field_(beg_ts, _key_info.pow.min,    ki.pow.min);
  this->set_field_(beg_ts, _key_info.pow.perc50, ki.pow.perc50);
  this->set_field_(beg_ts, _key_info.pow.perc80, ki.pow.perc80);
  this->set_field_(beg_ts, _key_info.pow.perc95, ki.pow.perc95);
  this->set_field_(beg_ts, _key_info.pow.perc99, ki.pow.perc99);
  this->set_field_(beg_ts, _key_info.pow.perc100,ki.pow.perc100);
  this->set_field_(beg_ts, _key_info.pow.max,    ki.pow.max);
  this->set_field_(beg_ts, _key_info.pow.avg,    ki.pow.avg);
  _key_info.last_update = ki.last_update;
  wrtstat::reduced_data rd;
  rd.ts = ki.count.ts;
  rd.count = ki.count.value; 
  rd.lossy = ki.lossy.value;
  rd.min = ki.max.min.value;
  rd.max  = ki.max.max.value;
  rd.avg = ki.max.avg.value;
  _aggregator.add(rd);
}

aggregated_info key_aggregator::get_aggregated_info(bool pow) const
{
  std::lock_guard<mutex_type> lk(_mutex);
  aggregated_info ai;
  ai.count = _key_info.count.value;
  ai.lossy = _key_info.lossy.value;
  if (!pow)
  {
    ai.min = _key_info.max.min.value;
    ai.perc50 = _key_info.max.perc50.value;
    ai.perc80 = _key_info.max.perc80.value;
    ai.perc95 = _key_info.max.perc95.value;
    ai.perc99 = _key_info.max.perc99.value;
    ai.perc100 = _key_info.max.perc100.value;
    ai.max = _key_info.max.max.value;
    ai.avg = _key_info.max.avg.value;
  }
  else
  {
    ai.min = _key_info.pow.min.value;
    ai.perc50 = _key_info.pow.perc50.value;
    ai.perc80 = _key_info.pow.perc80.value;
    ai.perc95 = _key_info.pow.perc95.value;
    ai.perc99 = _key_info.pow.perc99.value;
    ai.perc100 = _key_info.pow.perc100.value;
    ai.max = _key_info.pow.max.value;
    ai.avg = _key_info.pow.avg.value;
  }
  return ai;
}

void key_aggregator::add_handler_(std::vector<aggregated_info>* up_data, aggregated_ptr ag)
{
  //if ( ag->ts == 0 ) abort();
  typedef wrtstat::reduced_info reduced_info;
  typedef wrtstat::aggregated_perc aggregated_perc;
  aggregated_info ai;
  static_cast<reduced_info&>( ai ) = static_cast<reduced_info&>(*ag);
  static_cast<aggregated_perc&>( ai ) = static_cast<aggregated_perc&>(*ag);
  this->update_info_( ai );
  if ( up_data!=nullptr )
    up_data->push_back( std::move(ai));
}

void key_aggregator::update_info_(const aggregated_info& data)
{
  _key_info.last_update = data.ts;
  this->update_field_(_key_info.count, data.count, data.ts);
  this->update_field_(_key_info.lossy, data.lossy, data.ts);
  
  this->update_field_(_key_info.max.min,     data.min, data.ts);
  this->update_field_(_key_info.max.perc50,  data.perc50, data.ts);
  this->update_field_(_key_info.max.perc80,  data.perc80, data.ts);
  this->update_field_(_key_info.max.perc95,  data.perc95, data.ts);
  this->update_field_(_key_info.max.perc99,  data.perc99, data.ts);
  this->update_field_(_key_info.max.perc100, data.perc100, data.ts);
  this->update_field_(_key_info.max.max,     data.max, data.ts);
  this->update_field_(_key_info.max.avg,     data.avg, data.ts);

  this->update_field_(_key_info.pow.min,     data.min*data.count, data.ts);
  this->update_field_(_key_info.pow.perc50,  data.perc50*data.count, data.ts);
  this->update_field_(_key_info.pow.perc80,  data.perc80*data.count, data.ts);
  this->update_field_(_key_info.pow.perc95,  data.perc95*data.count, data.ts);
  this->update_field_(_key_info.pow.perc99,  data.perc99*data.count, data.ts);
  this->update_field_(_key_info.pow.perc100, data.perc100*data.count, data.ts);
  this->update_field_(_key_info.pow.max,     data.max*data.count, data.ts);
  this->update_field_(_key_info.pow.avg,     data.avg*data.count, data.ts);

}
  
void key_aggregator::update_field_(stored_pair& field, value_type v, time_type t)
{
  if ( field.value < v )
  {
    field.value = v;
    field.ts = t;
  }
}

void key_aggregator::set_field_(time_type beg_ts, stored_pair& f1, const stored_pair& f2)
{
  if ( f1.ts < beg_ts || f1.value < f2.value )
  {
    f1.value = f2.value;
    f1.ts = f2.ts;
  }
}

}}
