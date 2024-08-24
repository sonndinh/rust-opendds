#include "rust-opendds/include/rust-opendds.h"

#include <vector>
#include <stdexcept>
#include <iostream>

using namespace std;

namespace OpenDDS {

DDS::DomainParticipantFactory_var dpf_;
std::vector<DDS::DomainParticipant_var> participants_;

void initialize(int argc, rust::Vec<rust::String> argv)
{
  char* argv_str[argv.size()];
  for (size_t i = 0; i < argv.size(); ++i) {
    argv_str[i] = const_cast<char*>(argv[i].c_str());
  }
  dpf_ = TheParticipantFactoryWithArgs(argc, argv_str);
  cout << "From C++: initialize" << endl;
}

const DDS::DomainParticipant_var& create_participant(int domain_id)
{
  DDS::DomainParticipantQos qos;
  dpf_->get_default_participant_qos(qos);
  DDS::DomainParticipant_var dp = dpf_->create_participant(domain_id, qos, 0, 0);
  if (!dp) {
    throw runtime_error("create_participant failed");
  }
  auto ret = participants_.insert(participants_.end(), dp);
  cout << "From C++: create_participant" << endl;
  return *ret;
}

void delete_participant(const DDS::DomainParticipant_var& dp)
{
  std::vector<DDS::DomainParticipant_var>::iterator iter =
    std::find(participants_.begin(), participants_.end(), dp);
  if (iter != participants_.end()) {
    participants_.erase(iter);
  }
  dp->delete_contained_entities();
  dpf_->delete_participant(dp);
  cout << "From C++: delete_participant" << endl;
}

void subscribe()
{
}

void unsubscribe()
{
}

void create_datawriter()
{
}

void write()
{
}

}
