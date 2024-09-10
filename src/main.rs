// A simple application that contains both a subscriber and a publisher.
use rust_opendds::ffi;
use std::{thread, time};

fn rust_callback(si: ffi::SampleInfo, sample: String) {
    if si.valid_data {
        println!("Rust: Received a sample with VALID data: {}", sample);
    } else {
        println!("Rust: Received a sample with INVALID data: {}", sample);
    }
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

    let cb: fn(ffi::SampleInfo, String) = rust_callback;
    ffi::subscribe(&dp, "topic".to_string(), "Messenger::Message".to_string(), cb);

    let dwi = ffi::create_datawriter(&dp, "topic".to_string(), "Messenger::Message".to_string());
    ffi::wait_for_readers(&dwi);

    let sample = r#"{ "from": "Alice", "subject": "Hello from Alice", "subject_id": 0, "text": "Wanna play soccer today", "count": 2 }"#;
    ffi::write(&dwi, sample.to_string(), 0);

    // Wait for the reader to receive the samples
    thread::sleep(time::Duration::from_millis(3000));

    ffi::delete_participant(dp);
}
