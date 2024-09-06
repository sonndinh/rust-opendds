use std::{thread, time};

#[cxx::bridge(namespace = "Rust_OpenDDS")]
mod ffi {
    extern "Rust" {
        fn rust_callback(sample: String);
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
        fn create_datawriter(dp: &UniquePtr<DomainParticipantVar>, topic_name: String, type_name: String) -> UniquePtr<DataWriterInfo>;
        fn wait_for_readers(dwi: &UniquePtr<DataWriterInfo>);
        fn write(dwi: &UniquePtr<DataWriterInfo>, sample: String, instance_handle: i32);
    }
}

pub fn rust_callback(sample: String) {
    println!("Rust: Received a sample: {}", sample);
}

fn main() {
    println!("Hello from Rust main");
    let domain_id: i32 = 32;
    ffi::initialize(4, vec!["-DCPSDebugLevel".to_string(),
                            "1".to_string(),
                            "-DCPSConfigFile".to_string(),
                            "rtps_disc.ini".to_string()]);
    ffi::load("/Users/sonndinh/Codes/OpenDDS/examples/DCPS/Messenger_Imr/DDS_Messenger_Imr_Idl".to_string());
    let dp = ffi::create_participant(domain_id);
    ffi::subscribe(&dp, "topic".to_string(), "Messenger::Message".to_string());

    let dwi = ffi::create_datawriter(&dp, "topic".to_string(), "Messenger::Message".to_string());
    ffi::wait_for_readers(&dwi);

    let sample = r#"{ "from": "Alice", "subject": "Hello from Alice", "subject_id": 0, "text": "Blah blah blah", "count": 2 }"#;
    ffi::write(&dwi, sample.to_string(), 0);

    // Wait for the reader to receive the samples
    thread::sleep(time::Duration::from_millis(3000));

    ffi::delete_participant(dp);
}
