#include <fas/testing.hpp>
#include <btp/key_aggregator.hpp>

namespace{

  std::mutex s_mutex;
  using namespace ::wamba::btp;
  typedef wrtstat::time_type time_type;
  struct _storage_;
  struct _data1_;
  struct _data2_;

  template<typename T>
  void test_data1(T& t, const aggregated_info& ai, time_type ts, int factor)
  {
    using namespace fas::testing;
    t << equal<expect, time_type>( ai.ts, ts) << FAS_FL;
    t << equal<expect, size_t>( ai.min, 11) << FAS_FL;
    t << equal<expect, size_t>( ai.max, 1001) << FAS_FL;
    t << equal<expect, size_t>( ai.count, 101*factor) << FAS_FL;
    //t << equal<expect, size_t>( ai.lossy, 899*factor) << FAS_FL;
    t << equal<expect, size_t>( ai.perc50, 501) << FAS_FL;
    t << equal<expect, size_t>( ai.perc80, 801) << FAS_FL;
    t << equal<expect, size_t>( ai.perc95, 951) << FAS_FL;
    t << equal<expect, size_t>( ai.perc99, 991) << FAS_FL;
    t << equal<expect, size_t>( ai.perc100, 1001) << FAS_FL;
  }

  /*
  template<typename T>
  void test_data2(T& t, const aggregated_info& ai, time_type ts)
  {
    using namespace fas::testing;
    t << equal<expect, time_type>( ai.ts, ts) << FAS_FL;
    t << equal<expect, size_t>( ai.min, 0) << FAS_FL;
    t << equal<expect, size_t>( ai.max, 1001) << FAS_FL;
    t << equal<expect, size_t>( ai.count, 101) << FAS_FL;
    t << equal<expect, size_t>( ai.lossy, 899) << FAS_FL;
    t << equal<expect, size_t>( ai.perc50, 500) << FAS_FL;
    t << equal<expect, size_t>( ai.perc80, 800) << FAS_FL;
    t << equal<expect, size_t>( ai.perc95, 950) << FAS_FL;
    t << equal<expect, size_t>( ai.perc99, 990) << FAS_FL;
    t << equal<expect, size_t>( ai.perc100, 999) << FAS_FL;
  }*/
  
  template<typename T>
  void test_info1(T& t, const stored_values& ai, time_type ts, int factor)
  {
    using namespace fas::testing;
    t << equal<expect, time_type>( ai.last_update, ts) << FAS_FL;
    t << equal<expect, size_t>( ai.max.min.value, 11) << FAS_FL;
    t << equal<expect, size_t>( ai.max.max.value, 1001) << FAS_FL;
    t << equal<expect, size_t>( ai.count.value, 101*factor) << FAS_FL;
    //t << equal<expect, size_t>( ai.lossy.value, 899*factor) << FAS_FL;
    t << equal<expect, size_t>( ai.max.perc50.value, 501) << FAS_FL;
    t << equal<expect, size_t>( ai.max.perc80.value, 801) << FAS_FL;
    t << equal<expect, size_t>( ai.max.perc95.value, 951) << FAS_FL;
    t << equal<expect, size_t>( ai.max.perc99.value, 991) << FAS_FL;
    t << equal<expect, size_t>( ai.max.perc100.value, 1001) << FAS_FL;
  }


  void init1(aggregated_data& value)
  {
    value.ts = 1;
    value.min = 11;
    value.count = 101;
    value.lossy = 899;
    value.perc50 = 501;
    value.perc80 = 801;
    value.perc95 = 951;
    value.perc99 = 991;
    value.perc100 = 1001;
    value.max = 1001;
    value.data= {};
  }
 
 /*
  void init2(aggregated_data& value)
  {
    init1(value);
    for (int i=0; i < 1000; ++i)
      value.data.push_back(i);
  }
  */
 
  UNIT(ini1, "")
  {
    using namespace fas::testing;
    
    std::shared_ptr<key_aggregator>& stg = GET_REF(_storage_);
    aggregated_data& value1 = GET_REF(_data1_);
    aggregated_data& value2 = GET_REF(_data2_);
        
    key_aggregator_options options;
    options.reducer_limit  = 2048;
    options.reducer_levels = 16;
    stg = std::make_shared<key_aggregator>(s_mutex, 1/*id*/, 1/*ts*/, options);
    
    init1(value1);
    init1(value2);
  }

  UNIT(add1, "Данные поступают с интервалом шага аггрегации")
  {
    using namespace fas::testing;

    std::shared_ptr<key_aggregator>& stg = GET_REF(_storage_);
    aggregated_data& value1 = GET_REF(_data1_);
    
    stored_values up_key;
    std::vector<aggregated_info> up_data;
  
    aggregated_data v = value1; v.ts = 2;
    bool is_last = false;
    stg->add( std::move(v), &up_data, &is_last );
    t << equal<assert, size_t>( 0, up_data.size() ) << FAS_FL;
    t << stop;
  
    for (time_type i = 3; i < 32; ++i)
    {
      v = value1; v.ts = i;
      up_data.clear();
      stg->add( std::move(v), &up_data, &is_last );
      up_key=stg->get_key_info();
      t << equal<assert, size_t>( 1, up_data.size() ) << FAS_FL;
      t << stop;
      //t << message("i=") << i << ", key_info=" << up_key.max.max.value << " " << FAS_FL;
      test_data1( t, up_data.back(), i-1, 1);
      test_info1( t, up_key, i-1, 1);
    }
    return;
  }
  
  UNIT(ini2, "")
  {
    using namespace fas::testing;
    
    std::shared_ptr<key_aggregator>& stg = GET_REF(_storage_);
    aggregated_data& value1 = GET_REF(_data1_);
    aggregated_data& value2 = GET_REF(_data2_);
        
    key_aggregator_options options;
    options.aggregation_step_ts = 2;
    options.reducer_limit  = 2048;
    options.reducer_levels = 16;
    stg = std::make_shared<key_aggregator>(s_mutex, 1/*id*/, 2/*ts*/, options);
    
    init1(value1);
    init1(value2);
  }

  UNIT(add2, "Данные поступают с интервалом 1/2 шага аггрегации")
  {
    using namespace fas::testing;

    std::shared_ptr<key_aggregator>& stg = GET_REF(_storage_);
    aggregated_data& value1 = GET_REF(_data1_);
    
    stored_values up_key;
    std::vector<aggregated_info> up_data;
  
    aggregated_data v = value1; v.ts = 2;
    bool is_last = false;
    stg->add( std::move(v), &up_data, &is_last );
    t << equal<assert, size_t>( 0, up_data.size() ) << FAS_FL;
    t << stop;
    
    v = value1; v.ts = 3;
    up_data.clear();
    stg->add( std::move(v), &up_data, &is_last );
    t << equal<expect, size_t>( 0, up_data.size() ) <<  FAS_FL;
    t << stop;

    
    for (time_type i = 4; i < 32; ++i)
    {
      v = value1; v.ts = i;
      up_data.clear();
      stg->add( std::move(v), &up_data, &is_last );
      up_key=stg->get_key_info();
      if ( i % 2 ==0 )
      {
        //t << message("i=") << i << " " << FAS_FL;
        t << equal<assert, size_t>( 1, up_data.size() ) << FAS_FL;
        t << stop;
        test_data1( t, up_data.back(), i-2, 2);
        test_info1( t, up_key, i-2, 2);
      }
    }
    return;
  }
  
  UNIT(ini3, "")
  {
    using namespace fas::testing;
    
    std::shared_ptr<key_aggregator>& stg = GET_REF(_storage_);
    aggregated_data& value1 = GET_REF(_data1_);
    aggregated_data& value2 = GET_REF(_data2_);
        
    key_aggregator_options options;
    options.reducer_limit  = 2048;
    options.reducer_levels = 16;
    options.aggregate_last_point = true;
    stg = std::make_shared<key_aggregator>(s_mutex, 1/*id*/, 1/*ts*/, options);
    
    init1(value1);
    init1(value2);
  }

  UNIT(add3, "С включенной аггрегацией последней точки")
  {
    using namespace fas::testing;

    std::shared_ptr<key_aggregator>& stg = GET_REF(_storage_);
    aggregated_data& value1 = GET_REF(_data1_);
    
    stored_values up_key;
    std::vector<aggregated_info> up_data;
  
    aggregated_data v = value1; v.ts = 1;
    bool is_last = false;
    stg->add( std::move(v), &up_data, &is_last );
    t << equal<assert, size_t>( 1, up_data.size() ) << FAS_FL;
    t << stop;
  
    for (time_type i = 2; i < 32; ++i)
    {
      v = value1; v.ts = i;
      up_data.clear();
      stg->add( std::move(v), &up_data, &is_last );
      up_key=stg->get_key_info();
      t << equal<assert, size_t>( 2, up_data.size() ) << FAS_FL;
      t << stop;
      //t << message("i=") << i << " " << FAS_FL;
      test_data1( t, up_data.front(), i-1, 1);
      test_data1( t, up_data.back(), i, 1);
      test_info1( t, up_key, i, 1);
    }
    return;
  }
}

BEGIN_SUITE(key_aggregator, "")
  ADD_UNIT( ini1 )
  ADD_UNIT( add1 )
  ADD_UNIT( ini2 )
  ADD_UNIT( add2 )
  ADD_UNIT( ini3 )
  ADD_UNIT( add3 )
  ADD_VALUE(_storage_, std::shared_ptr<key_aggregator> )
  ADD_VALUE(_data1_, aggregated_data )
  ADD_VALUE(_data2_, aggregated_data )
END_SUITE(key_aggregator)
