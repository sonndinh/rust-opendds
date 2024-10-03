// For constants that don't need to cross the languages
// i.e. only used on the Rust side.
pub mod dds {
    #[allow(non_camel_case_types)]
    type ReturnCode_t = i32;
    pub const RETCODE_OK: ReturnCode_t = 0;
    pub const RETCODE_ERROR: ReturnCode_t = 1;
    pub const RETCODE_UNSUPPORTED: ReturnCode_t = 2;
    pub const RETCODE_BAD_PARAMETER: ReturnCode_t = 3;
    pub const RETCODE_PRECONDITION_NOT_MET: ReturnCode_t = 4;
    pub const RETCODE_OUT_OF_RESOURCES: ReturnCode_t = 5;
    pub const RETCODE_NOT_ENABLED: ReturnCode_t = 6;
    pub const RETCODE_IMMUTABLE_POLICY: ReturnCode_t = 7;
    pub const RETCODE_INCONSISTENT_POLICY: ReturnCode_t = 8;
    pub const RETCODE_ALREADY_DELETED: ReturnCode_t = 9;
    pub const RETCODE_TIMEOUT: ReturnCode_t = 10;
    pub const RETCODE_NO_DATA: ReturnCode_t = 11;
    pub const RETCODE_ILLEGAL_OPERATION: ReturnCode_t = 12;

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

    type StatusKind = u32;
    pub const INCONSISTENT_TOPIC_STATUS: StatusKind            = 0x0001 << 0;
    pub const OFFERED_DEADLINE_MISSED_STATUS: StatusKind       = 0x0001 << 1;
    pub const REQUESTED_DEADLINE_MISSED_STATUS: StatusKind     = 0x0001 << 2;
    pub const OFFERED_INCOMPATIBLE_QOS_STATUS: StatusKind      = 0x0001 << 5;
    pub const REQUESTED_INCOMPATIBLE_QOS_STATUS: StatusKind    = 0x0001 << 6;
    pub const SAMPLE_LOST_STATUS: StatusKind                   = 0x0001 << 7;
    pub const SAMPLE_REJECTED_STATUS: StatusKind               = 0x0001 << 8;
    pub const DATA_ON_READERS_STATUS: StatusKind               = 0x0001 << 9;
    pub const DATA_AVAILABLE_STATUS: StatusKind                = 0x0001 << 10;
    pub const LIVELINESS_LOST_STATUS: StatusKind               = 0x0001 << 11;
    pub const LIVELINESS_CHANGED_STATUS: StatusKind            = 0x0001 << 12;
    pub const PUBLICATION_MATCHED_STATUS: StatusKind           = 0x0001 << 13;
    pub const SUBSCRIPTION_MATCHED_STATUS: StatusKind          = 0x0001 << 14;

    type StatusMask = u32;
    pub const ALL_STATUS_MASK: StatusMask = 1u32 << 31;
    pub const NO_STATUS_MASK: StatusMask = 0;
    pub const DEFAULT_STATUS_MASK: StatusMask = ALL_STATUS_MASK;
}

#[cxx::bridge(namespace = "Rust_OpenDDS")]
pub mod ffi {
    struct ReturnCode_t {
        value: i32,
    }

    // Enable Rust application to pass custom Qos
    struct TopicDataQosPolicy {
        value: Vec<u8>, // DDS::OctetSeq
    }

    #[derive(Default)]
    struct UserDataQosPolicy {
        value: Vec<u8>, // DDS::OctetSeq
    }

    #[derive(Default)]
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

    #[derive(Default)]
    struct PresentationQosPolicy {
        access_scope: PresentationQosPolicyAccessScopeKind,
        coherent_access: bool,
        ordered_access: bool,
    }

    #[derive(Default)]
    struct PartitionQosPolicy {
        name: Vec<String>, // DDS::StringSeq
    }

    #[derive(Default)]
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

    #[derive(Default)]
    struct SubscriberQos {
        presentation: PresentationQosPolicy,
        partition: PartitionQosPolicy,
        group_data: GroupDataQosPolicy,
        entity_factory: EntityFactoryQosPolicy,
    }

    struct DomainParticipantFactoryQos {
        entity_factory: EntityFactoryQosPolicy,
    }

    #[derive(Default)]
    struct Property_t {
        name: String,
        value: String,
        propagate: bool,
    }

    #[derive(Default)]
    struct BinaryProperty_t {
        name: String,
        value: Vec<u8>, // DDS::OctetSeq
        propagate: bool,
    }

    #[derive(Default)]
    struct PropertyQosPolicy {
        value: Vec<Property_t>, // DDS::PropertySeq
        binary_value: Vec<BinaryProperty_t>, // DDS::BinaryPropertySeq
    }

    #[derive(Default)]
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
        type SubscriberVar;
        type DataReaderVar;
        type TopicVar;
        type DataWriterInfo;

        fn initialize(argc: i32, argv: Vec<String>);
        fn load(lib_path: String);

        // Domain participant
        fn get_default_participant_qos(qos: &mut DomainParticipantQos) -> ReturnCode_t;
        fn create_participant(domain_id: i32, qos: &DomainParticipantQos, mask: StatusMask) -> UniquePtr<DomainParticipantVar>;
        fn delete_participant(dp: UniquePtr<DomainParticipantVar>);

        // Topic
        fn get_default_topic_qos(dp: &UniquePtr<DomainParticipantVar>, qos: &mut TopicQos) -> ReturnCode_t;
        fn create_topic(dp: &UniquePtr<DomainParticipantVar>, topic_name: String, type_name: String, qos: &TopicQos, mask: StatusMask) -> UniquePtr<TopicVar>;

        // Subscriber
        fn get_default_subscriber_qos(dp: &UniquePtr<DomainParticipantVar>, qos: &mut SubscriberQos) -> ReturnCode_t;
        fn create_subscriber(dp: &UniquePtr<DomainParticipantVar>, qos: &SubscriberQos, mask: StatusMask) -> UniquePtr<SubscriberVar>;

        // Data reader
        fn get_default_datareader_qos(sub: &UniquePtr<SubscriberVar>, qos: &mut DataReaderQos) -> ReturnCode_t;
        fn create_datareader(sub: &UniquePtr<SubscriberVar>, topic: &UniquePtr<TopicVar>,
                             qos: &DataReaderQos, mask: StatusMask) -> UniquePtr<DataReaderVar>;
        fn set_listener(dr: &UniquePtr<DataReaderVar>, cb_fn: fn(si: SampleInfo, sample: String),
                        mask: StatusMask, dp: &UniquePtr<DomainParticipantVar>, type_name: String) -> ReturnCode_t;

        // TODO: Publisher

        // TODO: Data writer
        fn create_datawriter(dp: &UniquePtr<DomainParticipantVar>, topic_name: String, type_name: String) -> UniquePtr<DataWriterInfo>;
        fn write(dwi: &UniquePtr<DataWriterInfo>, sample: String, instance_handle: i32);

        // Invoked by writer
        fn wait_for_readers(dwi: &UniquePtr<DataWriterInfo>);
    }
}

impl Default for ffi::PresentationQosPolicyAccessScopeKind {
    fn default() -> Self
    {
        ffi::PresentationQosPolicyAccessScopeKind::INSTANCE_PRESENTATION_QOS
    }
}
