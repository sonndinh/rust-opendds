#include "rust/cxx.h"

#include <dds/DCPS/Service_Participant.h>

namespace OpenDDS {

typedef DDS::DomainParticipant_var DomainParticipantVar;

void initialize(int argc, rust::Vec<rust::String> argv);
const DDS::DomainParticipant_var& create_participant(int domain_id);
void delete_participant(const DDS::DomainParticipant_var& dp);
void subscribe();
void unsubscribe();
void create_datawriter();
void write();
}
