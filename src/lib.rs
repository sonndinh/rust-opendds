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

    #[allow(non_camel_case_types)]
    type TypeConsistencyEnforcementQosPolicyKind_t = i16;
    pub const DISALLOW_TYPE_COERCION: TypeConsistencyEnforcementQosPolicyKind_t = 1;
    pub const ALLOW_TYPE_COERCION: TypeConsistencyEnforcementQosPolicyKind_t = 2;
}

#[cxx::bridge(namespace = "Rust_OpenDDS")]
pub mod ffi {
    // Enable Rust application to pass custom Qos
    struct TopicDataQosPolicy {
        value: Vec<u8>, // DDS::OctetSeq
    }

    struct UserDataQosPolicy {
        value: Vec<u8>, // DDS::OctetSeq
    }

    struct GroupDataQosPolicy {
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

    struct OwnershipStrengthQosPolicy {
        value: i32,
    }

    struct WriterDataLifecycleQosPolicy {
        autodispose_unregistered_instances: bool,
    }

    struct DataWriterQos {
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
        user_data: UserDataQosPolicy,
        ownership: OwnershipQosPolicy,
        ownership_strength: OwnershipStrengthQosPolicy,
        writer_data_lifecycle: WriterDataLifecycleQosPolicy,
        representation: DataRepresentationQosPolicy,
    }

    enum PresentationQosPolicyAccessScopeKind {
        INSTANCE_PRESENTATION_QOS,
        TOPIC_PRESENTATION_QOS,
        GROUP_PRESENTATION_QOS,
    }

    struct PresentationQosPolicy {
        access_scope: PresentationQosPolicyAccessScopeKind,
        coherent_access: bool,
        ordered_access: bool,
    }

    struct PartitionQosPolicy {
        name: Vec<String>, // DDS::StringSeq
    }

    struct EntityFactoryQosPolicy {
        autoenable_created_entities: bool,
    }

    struct PublisherQos {
        presentation: PresentationQosPolicy,
        partition: PartitionQosPolicy,
        group_data: GroupDataQosPolicy,
        entity_factory: EntityFactoryQosPolicy,
    }

    struct TimeBasedFilterQosPolicy {
        minimum_separation: Duration_t,
    }

    struct ReaderDataLifecycleQosPolicy {
        autopurge_nowriter_samples_delay: Duration_t,
        autopurge_disposed_samples_delay: Duration_t,
    }

    struct TypeConsistencyEnforcementQosPolicy {
        kind: i16, // DDS::TypeConsistencyEnforcementQosPolicyKind_t
        ignore_sequence_bounds: bool,
        ignore_string_bounds: bool,
        ignore_member_names: bool,
        prevent_type_widening: bool,
        force_type_validation: bool,
    }

    struct DataReaderQos {
        durability: DurabilityQosPolicy,
        deadline: DeadlineQosPolicy,
        latency_budget: LatencyBudgetQosPolicy,
        liveliness: LivelinessQosPolicy,
        reliability: ReliabilityQosPolicy,
        destination_order: DestinationOrderQosPolicy,
        history: HistoryQosPolicy,
        resource_limits: ResourceLimitsQosPolicy,
        user_data: UserDataQosPolicy,
        ownership: OwnershipQosPolicy,
        time_based_filter: TimeBasedFilterQosPolicy,
        reader_data_lifecycle: ReaderDataLifecycleQosPolicy,
        representation: DataRepresentationQosPolicy,
        type_consistency: TypeConsistencyEnforcementQosPolicy,
    }

    struct SubscriberQos {
        presentation: PresentationQosPolicy,
        partition: PartitionQosPolicy,
        group_data: GroupDataQosPolicy,
        entity_factory: EntityFactoryQosPolicy,
    }

    struct DomainParticipantFactoryQos {
        entity_factory: EntityFactoryQosPolicy,
    }

    struct Property_t {
        name: String,
        value: String,
        propagate: bool,
    }

    struct BinaryProperty_t {
        name: String,
        value: Vec<u8>, // DDS::OctetSeq
        propagate: bool,
    }

    struct PropertyQosPolicy {
        value: Vec<Property_t>, // DDS::PropertySeq
        binary_value: Vec<BinaryProperty_t>, // DDS::BinaryPropertySeq
    }

    struct DomainParticipantQos {
        user_data: UserDataQosPolicy,
        entity_factory: EntityFactoryQosPolicy,
        property: PropertyQosPolicy, // DDS-Security 1.1 ptc/2017-09-26
    }

    // A better way would be type alias, i.e., type SampleStateKind = u32;
    // or tuple struct, i.e., struct SampleStateKind(u32);
    // However, cxx does not support these constructs, hence this work around.
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

    struct StatusMask {
        value: u32
    }

    unsafe extern "C++" {
        include!("rust-opendds/include/rust-opendds.h");

        type DomainParticipantVar;
        type DataWriterInfo;

        fn initialize(argc: i32, argv: Vec<String>);
        fn load(lib_path: String);
        fn create_participant(domain_id: i32, qos: DomainParticipantQos, mask: StatusMask) -> UniquePtr<DomainParticipantVar>;
        fn delete_participant(dp: UniquePtr<DomainParticipantVar>);
        fn subscribe(dp: &UniquePtr<DomainParticipantVar>, topic_name: String,
                     type_name: String, cb_fn: fn(si: SampleInfo, sample: String));
        fn create_datawriter(dp: &UniquePtr<DomainParticipantVar>, topic_name: String, type_name: String) -> UniquePtr<DataWriterInfo>;
        fn wait_for_readers(dwi: &UniquePtr<DataWriterInfo>);
        fn write(dwi: &UniquePtr<DataWriterInfo>, sample: String, instance_handle: i32);
    }
}
