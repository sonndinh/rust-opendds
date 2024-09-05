#[cxx::bridge(namespace = "Rust_OpenDDS")]
mod ffi {
    extern "Rust" {
        fn callback();
    }

    unsafe extern "C++" {
        include!("rust-opendds/include/rust-opendds.h");

        type DomainParticipantVar;
        type DataWriterInfo;

        fn initialize(argc: i32, argv: Vec<String>);
        fn load(lib_path: String);
        fn create_participant(domain_id: i32) -> UniquePtr<DomainParticipantVar>;
        fn delete_participant(dp: UniquePtr<DomainParticipantVar>);
        fn subscribe(dp: &UniquePtr<DomainParticipantVar>, topic_name: String, type_name: String);
        fn unsubscribe();
        fn create_datawriter(dp: &UniquePtr<DomainParticipantVar>, topic_name: String, type_name: String) -> UniquePtr<DataWriterInfo>;
        fn write(dwi: &UniquePtr<DataWriterInfo>, sample: String, instance_handle: i32);
    }
}

pub fn callback() {
    println!("Rust: Received a sample");
}

fn main() {
    println!("Hello from Rust main");
    let domain_id: i32 = 32;
    ffi::initialize(4, vec!["-DCPSDebugLevel".to_string(),
                            "3".to_string(),
                            "-DCPSConfigFile".to_string(),
                            "rtps_disc.ini".to_string()]);
    ffi::load("/Users/sonndinh/Codes/node-opendds/test/idl/NodeJSTest".to_string());
    let dp = ffi::create_participant(domain_id);
    ffi::create_datawriter(&dp, "topic".to_string(), "Mod::Sample".to_string());
    ffi::subscribe(&dp, "topic".to_string(), "Mod::Sample".to_string());
    ffi::delete_participant(dp);
}
