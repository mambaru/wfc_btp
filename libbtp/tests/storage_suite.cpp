#include <fas/testing.hpp>
#include <btp/storage.hpp>

namespace {

  using namespace ::wamba::btp;
  struct _storage_;

  UNIT(ini, "")
  {
    using namespace fas::testing;
    storage& stg = GET_REF(_storage_);
    storage_options opt;
    opt.trace = [&t](std::string msg)
    {
      t << message("TRACE: ") << msg;
    };
    std::string err;
    if ( !stg.open(opt, &err) )
    {
      t << error(err) << FAS_FL;
      return;
    }
    
    /*
    aggregated_data agd;
    std::string err;
    stg.add( "service~~service1~~server1~~op1", agd, &err );
    */
    t << nothing;
  }

  UNIT(set, "")
  {
    using namespace fas::testing;
    //storage& stg = GET_REF(_storage_);
    t << nothing;
  }

  UNIT(get, "")
  {
    using namespace fas::testing;
    //storage& stg = GET_REF(_storage_);
    t << nothing;
  }

}

BEGIN_SUITE(storage, "")
  ADD_UNIT( ini )
  ADD_UNIT( set )
  ADD_UNIT( get )
  ADD_VALUE(_storage_, storage)
END_SUITE(storage)
