#[cxx::bridge(namespace = "Rust_OpenDDS")]
pub mod ffi {
    struct SampleInfo {
        valid_data: bool,
    }

    // Ideally, we want to have a type for the callback function:
    // type CallbackFn = fn(SampleInfo, String);
    // However, cxx doesn't support this yet.

    unsafe extern "C++" {
        include!("rust-opendds/include/rust-opendds.h");

        type DomainParticipantVar;
        type DataWriterInfo;

        fn initialize(argc: i32, argv: Vec<String>);
        fn load(lib_path: String);
        fn create_participant(domain_id: i32) -> UniquePtr<DomainParticipantVar>;
        fn delete_participant(dp: UniquePtr<DomainParticipantVar>);
        fn subscribe(dp: &UniquePtr<DomainParticipantVar>, topic_name: String,
                     type_name: String, cb_fn: fn(si: SampleInfo, sample: String));
        fn create_datawriter(dp: &UniquePtr<DomainParticipantVar>, topic_name: String, type_name: String) -> UniquePtr<DataWriterInfo>;
        fn wait_for_readers(dwi: &UniquePtr<DataWriterInfo>);
        fn write(dwi: &UniquePtr<DataWriterInfo>, sample: String, instance_handle: i32);
    }
}
