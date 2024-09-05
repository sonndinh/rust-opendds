#include "rust-opendds/include/DataReaderListenerImpl.h"
#include "rust-opendds/src/main.rs.h"

namespace Rust_OpenDDS {

void DataReaderListenerImpl::on_data_available(DDS::DataReader*)
{
  ACE_DEBUG((LM_DEBUG, "C++: on_data_available called\n"));
  callback();
}

}
