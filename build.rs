fn main() {
    cxx_build::bridge("src/main.rs")
        .file("src/rust-opendds.cpp")
        .flag_if_supported("-std=c++11")
        .include("/Users/sonndinh/Codes/OpenDDS")
        .include("/Users/sonndinh/Codes/OpenDDS/ACE_wrappers")
        .include("/Users/sonndinh/Codes/OpenDDS/ACE_wrappers/TAO")
        .compile("cxxbridge-rust-opendds");

    println!("cargo:rerun-if-changed=src/main.rs");
    println!("cargo:rerun-if-changed=src/rust-opendds.cpp");
    println!("cargo:rerun-if-changed=include/rust-opendds.h");
    println!("cargo:rustc-link-search=/Users/sonndinh/Codes/OpenDDS/lib");
    println!("cargo:rustc-link-search=/Users/sonndinh/Codes/OpenDDS/ACE_wrappers/lib");
    println!("cargo:rustc-link-lib=OpenDDS_Dcps");
    println!("cargo:rustc-link-lib=ACE");
    println!("cargo:rustc-link-lib=TAO_Valuetype");
}
