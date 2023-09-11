#include <fas/testing.hpp>
#include <btp/key_cache.hpp>
#include <btp/types.hpp>

namespace{

  using namespace ::wamba::btp;
  typedef wrtstat::time_type time_type;
  struct _storage_;
  struct _data_;

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

  UNIT(ini1, "")
  {
    using namespace fas::testing;
    
    std::shared_ptr<key_cache>& stg = GET_REF(_storage_);
        
    key_cache_options options;
    options.reducer_limit  = 2048;
    options.reducer_levels = 16;
    options.aggregate_last_point = true;
    stg = std::make_shared<key_cache>(options);
    aggregated_data value;
    init1(value);

    for (int s=0; s < 10; s++)
    {
      for (int i=0; i < 10; i++)
      {
        for (int k=0; k < 10; k++)
        {
          std::stringstream ss;
          ss << "script~~script"<< s << "~~service" << i << "~~op" << k;
          key_id_t id1 = stg->create( ss.str(), 1 );
          t << not_equal<expect, key_id_t>( id1, 0 ) << FAS_FL;
          t << not_equal<expect, key_id_t>( id1, bad_key ) << FAS_FL;
        }
      }
    }
    
    for (int i=0; i < 10; i++)
    {
      for (int j=0; j < 10; j++)
      {
        for (int k=0; k < 10; k++)
        {
          std::stringstream ss;
          ss << "service~~service"<< i << "~~server" << j << "~~op" << k;
          aggregated_data v1(value);
          v1.perc100 = 1100 + k*5;
          bool is_last = false;
          key_id_t id1 = stg->add( ss.str(), std::move(v1), nullptr, nullptr, &is_last );
          t << not_equal<expect, key_id_t>( id1, 0 ) << FAS_FL;
          t << not_equal<expect, key_id_t>( id1, bad_key ) << FAS_FL;
        }
      }
    }

    
    for (int i=0; i < 10; i++)
    {
      for (int k=0; k < 10; k++)
      {
        std::stringstream ss;
        ss << "service~~service"<< i << "~~op" << k;
        bool is_last = false;
        key_id_t id1 = stg->add( ss.str(), aggregated_data(value), nullptr, nullptr, &is_last );
        t << not_equal<expect, key_id_t>( id1, 0 ) << FAS_FL;
        t << not_equal<expect, key_id_t>( id1, bad_key ) << FAS_FL;
      }
    }

    t << equal<expect, size_t>( stg->size(), 10*10*10 + 10*10*10 + 10*10) << FAS_FL;
  }
  
  UNIT(sel1, "")
  {
    using namespace fas::testing;
    std::shared_ptr<key_cache>& stg = GET_REF(_storage_);
    
    auto res = stg->select("", "", 777, 777, sort_by::NOSORT, false);
    t << equal<expect, size_t>( res.size(), 777) << FAS_FL;
    
    size_t pos = 0;
    for (int s=0; s < 10; s++)
    {
      for (int i=0; i < 10; i++)
      {
        for (int k=0; k < 10; k++)
        {
          if (pos>=777 && pos < 1554)
          {
            auto name = res[pos-777];
            std::stringstream ss;
            ss << "script~~script"<< s << "~~service" << i << "~~op" << k;
            t << equal<expect, std::string>( name.first, ss.str() ) << pos << FAS_FL;
          }
          pos++;
        }
      }
    }
  }
  
  UNIT(sel2, "")
  {
    using namespace fas::testing;
    std::shared_ptr<key_cache>& stg = GET_REF(_storage_);
    
    auto res = stg->select("service~~service2", "~~op7", 0, 200, sort_by::NOSORT, false);
    t << equal<assert, size_t>( res.size(), 11) << FAS_FL;
    t << equal<expect, std::string>( res[0].first, "service~~service2~~op7") << FAS_FL;
    t << equal<expect, std::string>( res[1].first, "service~~service2~~server0~~op7") << FAS_FL;
    t << equal<expect, std::string>( res[2].first, "service~~service2~~server1~~op7") << FAS_FL;
    t << equal<expect, std::string>( res[10].first, "service~~service2~~server9~~op7") << FAS_FL;
    /*for ( auto r : res )
      t << message(r);*/
    res = stg->select("service~~service2", "~~op7", 3, 1, sort_by::NOSORT, false);
    t << equal<assert, size_t>( res.size(), 1) << FAS_FL;
    t << equal<expect, std::string>( res[0].first, "service~~service2~~server2~~op7") << FAS_FL;
    
    res = stg->select("service~~service2~~server9", "", 0, 200, sort_by::PERC100, false);
    t << equal<assert, size_t>( res.size(), 10) << FAS_FL;
    t << equal<expect, std::string>( res[0].first, "service~~service2~~server9~~op9") << FAS_FL;
    t << equal<expect, std::string>( res[1].first, "service~~service2~~server9~~op8") << FAS_FL;
    t << equal<expect, std::string>( res[2].first, "service~~service2~~server9~~op7") << FAS_FL;
    //t << equal<expect, std::string>( res[10], "service~~service2~~op7") << FAS_FL;
    /*for ( auto r : res )
      t << message(r);*/
  }
  
  UNIT(three1, "")
  {
    using namespace fas::testing;
    std::shared_ptr<key_cache>& stg = GET_REF(_storage_);
    auto res1 = stg->tree("service", "~~", 1, 0, 0, tree_node::BRANCH, sort_by::PERC100, false );
    t << equal<expect, size_t>( res1.size(), 10) << FAS_FL;
    for ( auto r1 : res1 )
    {
      t << equal<expect, std::string>("service", r1.first.substr(0,7) ) << FAS_FL;
      auto res2 = stg->tree("service~~" + r1.first, "~~", 1, 0, 0, tree_node::BRANCH, sort_by::PERC100, false );
      t << equal<assert, size_t>( res2.size(), 10) << FAS_FL;

      for ( auto r2 : res2 )
      {
        // t << message("-->") << r2;
        t << equal<expect, std::string>("server", r2.first.substr(0,6) ) << FAS_FL;
        auto res3 = stg->tree("service~~" + r1.first + "~~" + r2.first, "~~", 1, 0, 0, tree_node::LEAF, sort_by::PERC100, false );
        for ( auto r3 : res3 )
        {
          //t << message("---->") << r3;
          t << equal<expect, std::string>("op", r3.first.substr(0,2) ) << FAS_FL;
        }
      }
      auto res4 = stg->tree("service~~" + r1.first, "~~", 1, 0, 0, tree_node::LEAF, sort_by::PERC100, false );
      t << equal<assert, size_t>( res4.size(), 10) << FAS_FL;

    }
  }
}

BEGIN_SUITE(key_cache, "")
  ADD_UNIT( ini1 )
  ADD_UNIT( sel1 )
  ADD_UNIT( sel2 )
  ADD_UNIT( three1 )
  ADD_VALUE(_storage_, std::shared_ptr<key_cache> )
END_SUITE(key_cache)
