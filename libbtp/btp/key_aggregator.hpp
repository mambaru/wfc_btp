#pragma once
#include <btp/types.hpp>
#include <btp/stored_key.hpp>
#include <btp/key_aggregator_options.hpp>
#include <wrtstat/aggregator/aggregator.hpp>
#include <functional>

namespace wamba{ namespace btp{

class key_aggregator
{
public:
  typedef key_aggregator_options options_type;
  typedef wrtstat::time_type time_type;
  typedef wrtstat::time_type value_type;
  typedef wrtstat::aggregator::aggregated_handler handler;
  typedef wrtstat::aggregated_data::ptr aggregated_ptr;
  
  key_aggregator() = delete;
  key_aggregator(const key_aggregator&) = delete;
  key_aggregator& operator=(const key_aggregator&) = delete;
  key_aggregator(key_aggregator&&) = delete;
  key_aggregator& operator=(key_aggregator&&) = delete;
  
  key_aggregator(mutex_type& m, key_id_t id, time_type ts, const options_type& opt);
  
  bool add(aggregated_data&& data, std::vector<aggregated_info>* up_data, bool* is_last);
  
  bool merge(aggregated_list&& data, std::vector<aggregated_info>* up_data);
  
  stored_key get_key_info() const;
  
  aggregated_info get_aggregated_info(bool pow) const;
  
  void set_key_info(const stored_values& ki);
  
  key_id_t get_id() const { return _key_info.id;};
  
  time_type get_last_update() const { return _key_info.last_update;}
  
  void aggregate_last_point_if(std::vector<aggregated_info>* up_data);
private:
  void add_handler_(std::vector<aggregated_info>* up_data, aggregated_ptr ag);
  void update_info_(const aggregated_info& data);
  void update_field_(stored_pair& field, value_type v, time_type t);
  void set_field_(time_type beg_ts, stored_pair& f1, const stored_pair& f2);
  void aggregate_last_point_if_(std::vector<aggregated_info>* up_data);
private:
  mutex_type& _mutex;
  stored_key _key_info;
  wrtstat::aggregator _aggregator;
  bool _aggregate_last_point = false;
};

}}
