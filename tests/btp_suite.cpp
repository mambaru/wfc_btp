#include <fas/testing.hpp>
#include <wfc/testing/testing_domain.hpp>
#include <ag/domain/ag_domain.hpp>


namespace {

UNIT(init, "")
{
  using namespace fas::testing;
  auto ptest = std::make_shared<wfc::testing_domain>();
  wamba::btp::ag_domain::domain_config conf;
  auto pdemo = ptest->create<wamba::btp::ag_domain>(conf);
  t << nothing;
}

}

BEGIN_SUITE(btp_suite, "")
  ADD_UNIT( init )
END_SUITE(btp_suite)
