#[cxx::bridge(namespace = "OpenDDS")]
mod ffi {
    unsafe extern "C++" {
        include!("rust-opendds/include/rust-opendds.h");

        //type DomainParticipantVar;
        type DomainParticipantHandle;

        fn initialize(argc: i32, argv: Vec<String>);
        fn load(lib_path: String);
        //fn create_participant(domain_id: i32) -> &'static mut DomainParticipantVar;
        fn create_participant(domain_id: i32) -> UniquePtr<DomainParticipantHandle>;
        //fn delete_participant(dp: &mut DomainParticipantVar);
        fn delete_participant(dp_handle: UniquePtr<DomainParticipantHandle>);
        fn subscribe(dp_handle: &UniquePtr<DomainParticipantHandle>,
                     topic_name: String, topic_type: String);
        fn unsubscribe();
        fn create_datawriter();
        fn write();
    }
}

fn main() {
    println!("Hello from Rust main");
    let domain_id: i32 = 32;
    ffi::initialize(4, vec!["-DCPSDebugLevel".to_string(),
                            "7".to_string(),
                            "-DCPSConfigFile".to_string(),
                            "rtps_disc.ini".to_string()]);
    ffi::load("/Users/sonndinh/Codes/node-opendds/test/idl/NodeJSTest".to_string());
    let dp = ffi::create_participant(domain_id);
    ffi::subscribe(&dp, "topic".to_string(), "Mod::Sample".to_string());
    ffi::delete_participant(dp);
}
