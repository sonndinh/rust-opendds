// For constants that don't need to cross the languages
// i.e. only used on the Rust side.
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

    #[allow(non_camel_case_types)]
    type DataRepresentationId_t = i16;
    pub const XCDR_DATA_REPRESENTATION: DataRepresentationId_t = 0;
    pub const XML_DATA_REPRESENTATION: DataRepresentationId_t = 1;
    pub const XCDR2_DATA_REPRESENTATION: DataRepresentationId_t = 2;
}

#[cxx::bridge(namespace = "Rust_OpenDDS")]
pub mod ffi {
    struct TopicDataQosPolicy {
        value: Vec<u8>, // DDS::OctetSeq
    }

    enum DurabilityQosPolicyKind {
        VOLATILE_DURABILITY_QOS,
        TRANSIENT_LOCAL_DURABILITY_QOS,
        TRANSIENT_DURABILITY_QOS,
        PERSISTENT_DURABILITY_QOS,
    }

    struct DurabilityQosPolicy {
        kind: DurabilityQosPolicyKind,
    }

    struct Duration_t {
        sec: i32,
        nanosec: u32,
    }

    enum HistoryQosPolicyKind {
        KEEP_LAST_HISTORY_QOS,
        KEEP_ALL_HISTORY_QOS,
    }

    struct DurabilityServiceQosPolicy {
        service_cleanup_delay: Duration_t,
        history_kind: HistoryQosPolicyKind,
        history_depth: i32,
        max_samples: i32,
        max_instances: i32,
        max_samples_per_instance: i32,
    }

    struct DeadlineQosPolicy {
        period: Duration_t,
    }

    struct LatencyBudgetQosPolicy {
        duration: Duration_t,
    }

    enum LivelinessQosPolicyKind {
        AUTOMATIC_LIVELINESS_QOS,
        MANUAL_BY_PARTICIPANT_LIVELINESS_QOS,
        MANUAL_BY_TOPIC_LIVELINESS_QOS,
    }

    struct LivelinessQosPolicy {
        kind: LivelinessQosPolicyKind,
        lease_duration: Duration_t,
    }

    enum ReliabilityQosPolicyKind {
        BEST_EFFORT_RELIABILITY_QOS,
        RELIABLE_RELIABILITY_QOS,
    }

    struct ReliabilityQosPolicy {
        kind: ReliabilityQosPolicyKind,
        max_blocking_time: Duration_t,
    }

    enum DestinationOrderQosPolicyKind {
        BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS,
        BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS,
    }

    struct DestinationOrderQosPolicy {
        kind: DestinationOrderQosPolicyKind,
    }

    struct HistoryQosPolicy {
        kind: HistoryQosPolicyKind,
        depth: i32,
    }

    struct ResourceLimitsQosPolicy {
        max_samples: i32,
        max_instances: i32,
        max_samples_per_instance: i32,
    }

    struct TransportPriorityQosPolicy {
        value: i32,
    }

    struct LifespanQosPolicy {
        duration: Duration_t,
    }

    enum OwnershipQosPolicyKind {
        SHARED_OWNERSHIP_QOS,
        EXCLUSIVE_OWNERSHIP_QOS,
    }

    struct OwnershipQosPolicy {
        kind: OwnershipQosPolicyKind,
    }

    struct DataRepresentationQosPolicy {
        value: Vec<i16>, // DDS::DataRepresentationIdSeq
        //value: Vec<dds::DataRepresentationId_t>,
    }

    struct TopicQos {
        topic_data: TopicDataQosPolicy,
        durability: DurabilityQosPolicy,
        durability_service: DurabilityServiceQosPolicy,
        deadline: DeadlineQosPolicy,
        latency_budget: LatencyBudgetQosPolicy,
        liveliness: LivelinessQosPolicy,
        reliability: ReliabilityQosPolicy,
        destination_order: DestinationOrderQosPolicy,
        history: HistoryQosPolicy,
        resource_limits: ResourceLimitsQosPolicy,
        transport_priority: TransportPriorityQosPolicy,
        lifespan: LifespanQosPolicy,
        ownership: OwnershipQosPolicy,
        representation: DataRepresentationQosPolicy,
    }

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
