#[cxx::bridge(namespace = "OpenDDS")]
mod ffi {
    unsafe extern "C++" {
        include!("rust-opendds/include/rust-opendds.h");

        fn create_participant(argc: i32, argv: Vec<String>);
        //fn delete_participant();
        //fn subscribe();
        //fn unsubscribe();
        //fn create_datawriter();
        //fn write();
    }
}

fn main() {
    println!("Hello from Rust main");
    ffi::create_participant(4, vec!["-DCPSDebugLevel".to_string(),
                                    "7".to_string(),
                                    "-DCPSConfigFile".to_string(),
                                    "rtps_disc.ini".to_string()]);
}
