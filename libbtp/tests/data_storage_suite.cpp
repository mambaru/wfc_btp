#include <fas/testing.hpp>
#include <btp/data_storage.hpp>

namespace{
  using namespace ::wamba::btp;
  struct _storage_;
  
  void ini_info(aggregated_info& info, value_type value)
  {
    info.ts = value;
    
    info.avg=value+3;    
    info.count=value+5;  
    info.lossy=value+7;  
    info.min=value+9;    
    info.perc50=value+11;
    info.perc80=value+13;
    info.perc95=value+15;
    info.perc99=value+17;
    info.perc100=value+19;
    info.max=value+21;
  }

  template<typename T>
  void check_info(T& t, const aggregated_info& info, value_type value)
  {
    using namespace fas::testing;
    t << message("check ") << value;
    t << equal<expect, value_type>(info.ts, value) << FAS_FL;
    t << equal<expect, value_type>(info.avg,value+3) << FAS_FL;     
    t << equal<expect, value_type>(info.count,value+5) << FAS_FL;   
    t << equal<expect, value_type>(info.lossy,value+7) << FAS_FL;   
    t << equal<expect, value_type>(info.min,value+9) << FAS_FL;     
    t << equal<expect, value_type>(info.perc50,value+11) << FAS_FL;  
    t << equal<expect, value_type>(info.perc80,value+13) << FAS_FL;
    t << equal<expect, value_type>(info.perc95,value+15) << FAS_FL;
    t << equal<expect, value_type>(info.perc99,value+17) << FAS_FL;
    t << equal<expect, value_type>(info.perc100,value+19) << FAS_FL;
    t << equal<expect, value_type>(info.max,value+21) << FAS_FL;
  }

  UNIT(ini1, "")
  {
    using namespace fas::testing;
    auto& stg = GET_REF(_storage_);
    stg = std::make_shared<data_storage>();
    data_storage_options opt;
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
      for (key_id_t i = 100; i < 10000; i+=100)
      {
        aggregated_info info;
        ini_info(info, i);
        res = stg->set(i, info, &err);
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
    /*t << nothing;*/
    
    
    auto& stg = GET_REF(_storage_);
    for (value_type i = 100; i < 10000; i+=100)
    {
      std::string err;
      aggregated_list lst;
      bool res = stg->get(i, &lst, &err, 0, 0, 0);
      t << is_true<expect>( res ) << err << FAS_FL;
      t << is_false<expect>( lst.empty() ) <<  FAS_FL;
      check_info( t, lst[0], i);
    }
    
  }

}

BEGIN_SUITE(data_storage, "")
  ADD_UNIT( ini1 )
  ADD_UNIT( tst1 )
  ADD_VALUE(_storage_, std::shared_ptr<data_storage> )
END_SUITE(data_storage)
