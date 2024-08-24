#[cxx::bridge(namespace = "OpenDDS")]
mod ffi {
    unsafe extern "C++" {
        include!("rust-opendds/include/rust-opendds.h");

        type DomainParticipantVar;

        fn initialize(argc: i32, argv: Vec<String>);
        fn create_participant(domain_id: i32) -> &'static DomainParticipantVar;
        fn delete_participant(dp: &DomainParticipantVar);
        fn subscribe();
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
    let dp = ffi::create_participant(domain_id);
    ffi::delete_participant(dp);
}
