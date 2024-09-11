// A simple application that contains both a subscriber and a publisher.
use rust_opendds::ffi;
use rust_opendds::dds;
use std::{thread, time};

fn dump_sample_info(si: &ffi::SampleInfo) {
    let sample_state = if si.sample_state.value == dds::READ_SAMPLE_STATE {
        "READ_SAMPLE_STATE"
    } else {
        "NOT_READ_SAMPLE_STATE"
    };

    let view_state = if si.view_state.value == dds::NEW_VIEW_STATE {
        "NEW_VIEW_STATE"
    } else {
        "NOT_NEW_VIEW_STATE"
    };

    let instance_state = if si.instance_state.value == dds::ALIVE_INSTANCE_STATE {
        "ALIVE_INSTANCE_STATE"
    } else if si.instance_state.value == dds::NOT_ALIVE_DISPOSED_INSTANCE_STATE {
        "NOT_ALIVE_DISPOSED_INSTANCE_STATE"
    } else {
        "NOT_ALIVE_NO_WRITERS_INSTANCE_STATE"
    };

    println!("SampleInfo: {{");
    println!("\tsample_state: {}", sample_state );
    println!("\tview_state: {}", view_state);
    println!("\tinstance_state: {}", instance_state);
    println!("\tsource_timestamp: {{ {}, {} }}", si.source_timestamp.sec, si.source_timestamp.nanosec);
    println!("\tinstance_handle: {}", si.instance_handle.value);
    println!("\tpublication_handle: {}", si.publication_handle.value);
    println!("\tdisposed_generation_count: {}", si.disposed_generation_count);
    println!("\tno_writers_generation_count: {}", si.no_writers_generation_count);
    println!("\tsample_rank: {}", si.sample_rank);
    println!("\tgeneration_rank: {}", si.generation_rank);
    println!("\tabsolute_generation_rank: {}", si.absolute_generation_rank);
    println!("\tvalid_data: {}", si.valid_data);
    println!("\topendds_reserved_publication_seq: {}", si.opendds_reserved_publication_seq);
    println!("}}");
}

fn rust_callback(si: ffi::SampleInfo, sample: String) {
    dump_sample_info(&si);
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
