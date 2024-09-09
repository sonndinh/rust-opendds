use std::env;

fn main() {
    let dds_root = env::var("DDS_ROOT").expect("DDS_ROOT environment variable is not defined");
    let ace_root = env::var("ACE_ROOT").expect("ACE_ROOT environment variable is not defined");
    let tao_root = env::var("TAO_ROOT").expect("TAO_ROOT environment variable is not defined");
    let rapidjson_root = env::var("RAPIDJSON_ROOT").expect("RAPIDJSON_ROOT environment variable is not defined");
    let rapidjson_include_path = rapidjson_root + "/include";

    cxx_build::bridge("src/lib.rs")
        .file("src/rust-opendds.cpp")
        .file("src/DataReaderListenerImpl.cpp")
        .flag_if_supported("-std=c++11")
        .flag_if_supported("-DOPENDDS_RAPIDJSON")
        .include(dds_root.as_str())
        .include(ace_root.as_str())
        .include(tao_root.as_str())
        .include(rapidjson_include_path.as_str())
        .compile("cxxbridge-rust-opendds");

    println!("cargo:rerun-if-changed=src/main.rs");
    println!("cargo:rerun-if-changed=src/rust-opendds.cpp");
    println!("cargo:rerun-if-changed=include/rust-opendds.h");
    println!("cargo:rerun-if-changed=src/DataReaderListenerImpl.cpp");
    println!("cargo:rerun-if-changed=include/DataReaderListenerImpl.h");
    println!("cargo:rustc-link-search={}/lib", dds_root);
    println!("cargo:rustc-link-search={}/lib", ace_root);
    println!("cargo:rustc-link-search=/Users/sonndinh/Codes/OpenDDS/examples/DCPS/Messenger_Imr");
    println!("cargo:rustc-link-lib=OpenDDS_Dcps");
    println!("cargo:rustc-link-lib=ACE");
    println!("cargo:rustc-link-lib=TAO");
    println!("cargo:rustc-link-lib=TAO_Valuetype");
    println!("cargo:rustc-link-lib=DDS_Messenger_Imr_Idl");
}
