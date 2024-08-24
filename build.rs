use std::env;

fn main() {
    let dds_root = env::var("DDS_ROOT").expect("DDS_ROOT environment variable is not defined");
    let ace_root = env::var("ACE_ROOT").expect("ACE_ROOT environment variable is not defined");
    let tao_root = env::var("TAO_ROOT").expect("TAO_ROOT environment variable is not defined");
    
    cxx_build::bridge("src/main.rs")
        .file("src/rust-opendds.cpp")
        .flag_if_supported("-std=c++11")
        .include(dds_root.as_str())
        .include(ace_root.as_str())
        .include(tao_root.as_str())
        .compile("cxxbridge-rust-opendds");

    println!("cargo:rerun-if-changed=src/main.rs");
    println!("cargo:rerun-if-changed=src/rust-opendds.cpp");
    println!("cargo:rerun-if-changed=include/rust-opendds.h");
    println!("cargo:rustc-link-search={}/lib", dds_root);
    println!("cargo:rustc-link-search={}/lib", ace_root);
    println!("cargo:rustc-link-lib=OpenDDS_Dcps");
    println!("cargo:rustc-link-lib=ACE");
    println!("cargo:rustc-link-lib=TAO_Valuetype");
}
