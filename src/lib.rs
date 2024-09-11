pub mod dds {
    type SampleStateKind = u32;
    pub const READ_SAMPLE_STATE: SampleStateKind = 1u32 << 0;
    pub const NOT_READ_SAMPLE_STATE: SampleStateKind = 1u32 << 1;

    type ViewStateKind = u32;
    pub const NEW_VIEW_STATE: ViewStateKind = 1u32 << 0;
    pub const NOT_NEW_VIEW_STATE: ViewStateKind = 1u32 << 1;

    type InstanceStateKind = u32;
    pub const ALIVE_INSTANCE_STATE: InstanceStateKind = 1u32 << 0;
    pub const NOT_ALIVE_DISPOSED_INSTANCE_STATE: InstanceStateKind = 1u32 << 1;
    pub const NOT_ALIVE_NO_WRITERS_INSTANCE_STATE: InstanceStateKind = 1u32 << 2;
}

#[cxx::bridge(namespace = "Rust_OpenDDS")]
pub mod ffi {
    // Work around for cxx not supporting type alias and tuple struct.
    struct SampleStateKind {
        value: u32
    }

    struct ViewStateKind {
        value: u32
    }

    struct InstanceStateKind {
        value: u32
    }

    struct Time_t {
        sec: i32,
        nanosec: u32
    }

    struct InstanceHandle_t {
        value: i32
    }

    struct SampleInfo {
        sample_state: SampleStateKind,
        view_state: ViewStateKind,
        instance_state: InstanceStateKind,
        source_timestamp: Time_t,
        instance_handle: InstanceHandle_t,
        publication_handle: InstanceHandle_t,
        disposed_generation_count: i32,
        no_writers_generation_count: i32,
        sample_rank: i32,
        generation_rank: i32,
        absolute_generation_rank: i32,
        valid_data: bool,
        opendds_reserved_publication_seq: i64
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
