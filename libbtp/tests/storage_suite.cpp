#include <fas/testing.hpp>
#include <btp/storage.hpp>

namespace {

  using namespace ::wamba::btp;
  struct _storage_;
  time_t common_time = 0;
  
  UNIT(ini, "")
  {
    using namespace fas::testing;
    GET_REF(_storage_) = std::make_shared<storage>();
    storage& stg = *GET_REF(_storage_);
    storage_options opt;
    opt.key_cache.aggregate_last_point = true;
    opt.key_cache.aggregation_step_ts =  5;
    opt.data_db.TTL = 3600;
    opt.trace = [&t](std::string )
    {
      //t << message("TRACE: ") << msg;
    };
    common_time = time(nullptr);
    std::string err;
    if ( !stg.open(opt, &err) )
    {
      t << error(err) << FAS_FL;
      return;
    }
    
    aggregated_list agl;
    bool res = stg.get("service~~service1~~server1~~op1", &agl, &err, time(nullptr), 0, 1 );
    t << is_false<assert>(res) << FAS_FL;
  }

  UNIT(set, "")
  {
    using namespace fas::testing;
    storage& stg = *GET_REF(_storage_);
    aggregated_data agd;
    agd.count = 666;
    agd.ts = common_time;
    std::string err;
    bool res = stg.add("service~~service1~~server1~~op1", std::move(agd), &err );
    t << is_true<assert>(res) << FAS_FL;
  }

  UNIT(get, "")
  {
    using namespace fas::testing;
    storage& stg = *GET_REF(_storage_);
    aggregated_list agl;
    std::string err;
    bool res = stg.get("service~~service1~~server1~~op1", &agl, &err, common_time-600, 0, 0 );
    t << is_true<assert>(res) << FAS_FL;
    t << equal<assert, size_t>(agl.size(), 1) << FAS_FL;
  }
  
  UNIT(reinit, "")
  {
    using namespace fas::testing;
    GET_REF(_storage_) = std::make_shared<storage>();
    storage& stg = *GET_REF(_storage_);

    storage_options opt;
    opt.key_cache.aggregate_last_point = true;
    opt.key_cache.aggregation_step_ts =  5;
    opt.data_db.TTL = 3600;
    opt.trace = [&t](std::string )
    {
      //t << message("TRACE: ") << msg;
    };

    std::string err;
    if ( !stg.open(opt, &err) )
    {
      t << error(err) << FAS_FL;
      return;
    }
    
    aggregated_data agd;
    agd.count = 333;
    agd.ts = common_time;
    bool res = stg.add("service~~service1~~server1~~op1", std::move(agd), &err );
    t << is_true<assert>(res) << FAS_FL;
    t << stop;
    
    aggregated_list agl;
    res = stg.get("service~~service1~~server1~~op1", &agl, &err, time(nullptr)-600, 0, 0 );
    t << is_true<assert>(res) << FAS_FL;
    t << equal<assert, size_t>(agl.size(), 1) << FAS_FL;
    t << stop;
    t << equal<assert, size_t>(agl[0].count, 666+333) << FAS_FL;
    
  }

  UNIT(del, "")
  {
    using namespace fas::testing;
    storage& stg = *GET_REF(_storage_);
    std::string err;
    bool res = stg.del("service~~service1~~server1~~op1", &err);
    t << is_true<assert>(res) << err << FAS_FL;
    res = stg.del("service~~service1~~server1~~op1", &err);
    t << is_false<assert>(res) << err << FAS_FL;
  }
}

BEGIN_SUITE(storage, "")
  ADD_UNIT( ini )
  ADD_UNIT( set )
  ADD_UNIT( get )
  ADD_UNIT( reinit )  
  ADD_UNIT( del )
  ADD_VALUE(_storage_, std::shared_ptr<storage>)
END_SUITE(storage)
