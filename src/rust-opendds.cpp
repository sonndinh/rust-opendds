#include "rust-opendds/include/rust-opendds.h"

#include <dds/DCPS/Service_Participant.h>

#include <vector>
#include <stdexcept>
#include <iostream>

using namespace std;

namespace OpenDDS {

std::vector<DDS::DomainParticipant_var> participants_;

void create_participant(int argc, rust::Vec<rust::String> argv)
{
  DDS::DomainId_t domain = 32;
  cout << "Hello from C++" << endl;
  char* argv_str[argv.size()];
  for (size_t i = 0; i < argv.size(); ++i) {
    argv_str[i] = const_cast<char*>(argv[i].c_str());
  }
  DDS::DomainParticipantFactory_var dpf = TheParticipantFactoryWithArgs(argc, argv_str);
  DDS::DomainParticipantQos qos;
  dpf->get_default_participant_qos(qos);
  DDS::DomainParticipant_var dp = dpf->create_participant(domain, qos, 0, 0);
  if (!dp) {
    throw runtime_error("create_participant failed");
  }
  participants_.push_back(dp);
  cout << "Created a domain participant" << endl;
}

}
