#include <fas/testing.hpp>
#include <btp/key_storage.hpp>

namespace{
  using namespace ::wamba::btp;
  struct _storage_;
  
  void ini_info(stored_key& info, value_type value)
  {
    info.id = value;
    info.last_update = value+2;
    info.max.avg.value=value+3;     info.max.avg.ts = value+4;
    info.count.value=value+5;   info.count.ts = value+6;
    info.lossy.value=value+7;   info.lossy.ts = value+8;
    info.max.min.value=value+9;     info.max.min.ts = value+10;
    info.max.perc50.value=value+11;  info.max.perc50.ts = value+12;
    info.max.perc80.value=value+13;  info.max.perc80.ts = value+14;
    info.max.perc95.value=value+15;  info.max.perc95.ts = value+16;
    info.max.perc99.value=value+17;  info.max.perc99.ts = value+18;
    info.max.perc100.value=value+19; info.max.perc100.ts = value+20;
    info.max.max.value=value+21;     info.max.max.ts = value+22;
  }

  template<typename T>
  void check_info(T& t, const stored_key& info, value_type value)
  {
    using namespace fas::testing;
    // t << message("check ") << value;
    t << equal<expect, value_type>(info.id , value) << FAS_FL;
    t << equal<expect, value_type>(info.last_update , value+2) << FAS_FL;
    t << equal<expect, value_type>(info.max.avg.value,value+3) << FAS_FL;     
    t << equal<expect, value_type>(info.max.avg.ts , value+4) << FAS_FL;
    t << equal<expect, value_type>(info.count.value,value+5) << FAS_FL;   
    t << equal<expect, value_type>(info.count.ts , value+6) << FAS_FL;
    t << equal<expect, value_type>(info.lossy.value,value+7) << FAS_FL;   
    t << equal<expect, value_type>(info.lossy.ts , value+8) << FAS_FL;
    t << equal<expect, value_type>(info.max.min.value,value+9) << FAS_FL;     
    t << equal<expect, value_type>(info.max.min.ts , value+10) << FAS_FL;
    t << equal<expect, value_type>(info.max.perc50.value,value+11) << FAS_FL;  
    t << equal<expect, value_type>(info.max.perc50.ts , value+12) << FAS_FL;
    t << equal<expect, value_type>(info.max.perc80.value,value+13) << FAS_FL;
    t << equal<expect, value_type>(info.max.perc80.ts , value+14) << FAS_FL;
    t << equal<expect, value_type>(info.max.perc95.value,value+15) << FAS_FL;
    t << equal<expect, value_type>(info.max.perc95.ts , value+16) << FAS_FL;
    t << equal<expect, value_type>(info.max.perc99.value,value+17) << FAS_FL;
    t << equal<expect, value_type>(info.max.perc99.ts , value+18) << FAS_FL;
    t << equal<expect, value_type>(info.max.perc100.value,value+19) << FAS_FL;
    t << equal<expect, value_type>(info.max.perc100.ts , value+20) << FAS_FL;
    t << equal<expect, value_type>(info.max.max.value,value+21) << FAS_FL;
    t << equal<expect, value_type>(info.max.max.ts , value+22) << FAS_FL;
  }

  UNIT(ini1, "")
  {
    using namespace fas::testing;
    auto& stg = GET_REF(_storage_);
    stg = std::make_shared<key_storage>();
    key_storage_options opt;
    std::string err;
    opt.TTL = 5;
    bool res = stg->open(opt, &err);
    t << is_true<assert>(res) << "["<< err << "]" << FAS_FL;
    t << stop;

    res = stg->compact(&err);
    t << is_true<assert>(res) << "["<< err << "]" << FAS_FL;
    t << stop;

    if (t.get_argc() == 1 )
    {
      for (value_type i = 100; i < 10000; i+=100)
      {
        std::string name = std::string("key") + std::to_string(i);
        stored_key info;
        ini_info(info, i);
        res = stg->set(name, info, &err);
        t << is_true<assert>(res) << i << "["<< err << "]" << FAS_FL;
        t << stop;
      }
    }
    else
      t << message("Без заполнения хранилища");
  }
  
  UNIT(tst1, "")
  {
    using namespace fas::testing;
    auto& stg = GET_REF(_storage_);
    for (value_type i = 100; i < 10000; i+=100)
    {
      std::string err;
      std::string name = std::string("key") + std::to_string(i);
      stored_key info;
      bool res = stg->get(name, &info, &err);
      t << is_true<expect>( res ) << err << FAS_FL;
      check_info( t, info, i);
    }
  }

  UNIT(load1, "")
  {
    using namespace fas::testing;
    auto& stg = GET_REF(_storage_);
    size_t loadcounter = 0;
    stg->load(7, [&loadcounter, &t]( std::vector<stored_name> data)
    {
      //t << message("load: ") << data[0].name;
      if ( loadcounter != 98 )
        t << equal< expect, size_t>( data.size(), 7 ) << FAS_FL;
      else
        t << equal< expect, size_t>( data.size(), 1 ) << FAS_FL;
      loadcounter += data.size();
    }, nullptr);
    t << equal< expect, size_t>( loadcounter, 99 ) << FAS_FL;
  }
  
  UNIT(ini2, "")
  {
    using namespace fas::testing;
    auto& stg = GET_REF(_storage_);

    if (t.get_argc() == 1 )
    {
      std::string err;
      for (value_type i = 1; i < 10000; ++i)
      {
        std::string name = std::string("full") + std::to_string(i);
        stored_key info;
        ini_info(info, i);
        auto res = stg->set(name, info, &err);
        t << is_true<assert>(res) << i << "["<< err << "]" << FAS_FL;
        t << stop;
      }
    }
    else
      t << message("Без заполнения хранилища");
  }

}

BEGIN_SUITE(key_storage, "")
  ADD_UNIT( ini1 )
  ADD_UNIT( tst1 )
  ADD_UNIT( load1 )
  ADD_UNIT( ini2 )
  ADD_VALUE(_storage_, std::shared_ptr<key_storage> )
END_SUITE(key_storage)
