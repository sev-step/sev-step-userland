/**
 * @file sev_step_api.h
 * @author Luca Wilke (l.wilke@its.uni-luebeck.de)
 * @brief User space code for consuming the kernel API.
 * Move more complex functionality to sev_step_routines.{h,c}
 * @date 2023-01-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _SEV_STEP_API_H
#define _SEV_STEP_API_H


#include <stdint.h>
#include <stdbool.h>

#include "sev_step_error_codes.h"

#define KVMIO 0xAE


//
// Cache attack api types
//

/**
 * @brief struct for storing the performance counter config values
 */
typedef struct {
	uint64_t HostGuestOnly;
	uint64_t CntMask;
	uint64_t Inv;
	uint64_t En;
	uint64_t Int;
	uint64_t Edge;
	uint64_t OsUserMode;
	uint64_t UintMask;
	uint64_t EventSelect; //12 bits in total split in [11:8] and [7:0]
} perf_ctl_config_t;

/// @brief L2 Cache Hit From L1 Data Cache Miss Event for Host in OS Mode
extern perf_ctl_config_t host_os_l2_hit_from_l1_dc_miss;
extern perf_ctl_config_t host_os_l2_miss_from_l1_dc_miss;

extern perf_ctl_config_t host_ost_l2_cache_miss_from_l2_cache_hwpf_1;
extern perf_ctl_config_t host_ost_l2_cache_miss_from_l2_cache_hwpf_2;

extern perf_ctl_config_t host_os_l1d_fill_all;
extern perf_ctl_config_t host_os_l1d_fill_from_memory;
extern perf_ctl_config_t host_os_all_data_cache_accesses;
typedef struct {
    uint64_t lookup_table_index;
    bool apic_timer_value_valid;
    uint32_t custom_apic_timer_value;
} do_cache_attack_param_t;

typedef struct {
    /// @brief Input Parameter. We want the HPA for this
    uint64_t in_gpa;
    /// @brief Result Parameter.
    uint64_t out_hpa;
} gpa_to_hpa_param_t;

typedef struct {
    /// @brief guest vaddr where the lookup table starts
    uint64_t base_vaddr_table;
    /// @brief length of the lookup table in bytes
    uint64_t table_bytes;
} lookup_table_t;

typedef struct {
    /// @brief we build and l1d way predictor eviction for each target
    lookup_table_t* attack_targets;
    uint64_t attack_targets_len;
    /// @brief perf counter evaluated for cache attack
    perf_ctl_config_t cache_attack_perf;
} build_eviction_set_param_t;

typedef struct {
    /// @brief flattened 2D array with the evictions sets.
    /// Every @import_user_eviction_set_param_t.way_count elements form one eviction set
    /// for each cache set covered by the lookup_table
    uint64_t* eviction_sets;
    /// @brief length of eviction_sets
    uint64_t eviction_sets_len;
} lookup_table_eviction_set_t;

typedef struct {
    /// @brief we build and l1d way predictor eviction for each target
    lookup_table_t* attack_targets;
    /// @brief eviction sets for the supplied attack_targets
    lookup_table_eviction_set_t* eviction_sets;
    /// @brief len of both attack_targets and eviction_sets 
    uint64_t len;
     /// @brief ways of the attacked cache
    uint64_t way_count;
    /// @brief perf counter evaluated for cache attack
    perf_ctl_config_t cache_attack_perf;

} import_user_eviction_set_param_t;

//
// SEV-STEP Types
//


typedef enum {
    VRN_RFLAGS,
    VRN_RIP,
    VRN_RSP,
    VRN_R10,
    VRN_R11,
    VRN_R12,
    VRN_R13,
    VRN_R8,
    VRN_R9,
    VRN_RBX,
    VRN_RCX,
    VRN_RDX,
    VRN_RSI,
    VRN_CR3,
    VRN_MAX, //not a register; used to size sev_step_partial_vmcb_save_area_t.register_values
} vmsa_register_name_t;

/**
 * @brief Converts a register name from string to vmsa_register_name_t for
 * lookup in sev_step_partial_vmcb_save_area_t.register_values
 * 
 * @param c name of the register as str. Not case sensitive
 * @param result if SEV_STEP_OK, this contains the vmsa_register_name_t for the
 * register described by c
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int vmsa_register_name_from_str(char* c, vmsa_register_name_t* result);

typedef struct {
    /// @brief indexed by vmsa_register_name_t
    uint64_t register_values[VRN_MAX];
    bool failed_to_get_data;
} sev_step_partial_vmcb_save_area_t;

typedef struct {
	uint64_t gpa;
	int track_mode;
} track_page_param_t;

typedef struct {
	int track_mode;
} track_all_pages_t;

typedef enum {
    PAGE_FAULT_EVENT,
    SEV_STEP_EVENT,
} usp_event_type_t;

extern int SEV_STEP_SHARED_MEM_BYTES;
typedef struct {
    //lock for all of the other values in this struct
    int spinlock;
    //if true, we have a valid event stored 
    int have_event;
    //if true, the receiver has acked the event
    int event_acked;
    //type of the stored event. Required to do the correct raw mem cast
    usp_event_type_t event_type;
    uint8_t event_buffer[19 * 4096];
} shared_mem_region_t;

typedef struct {
    int pid;
    uint64_t user_vaddr_shared_mem;
    /// @brief if true, decrypt vmsa and send information with each event
    ///only works if debug mode is active
    bool decrypt_vmsa;
} usp_init_poll_api_t;

typedef struct {
    int pid;
    shared_mem_region_t* shared_mem_region;
    uint64_t next_id;
    int force_reset;
    
    //just for internal use. Used to remember get_user_pages_unlocked
    //page to be able to unpinn it on ctx destruction
    struct page* _page_for_shared_mem;
    int kvm_fd;
} usp_poll_api_ctx_t;

enum kvm_page_track_mode {
	KVM_PAGE_TRACK_WRITE,
	KVM_PAGE_TRACK_ACCESS,
	KVM_PAGE_TRACK_RESET_ACCESSED, //TODO: hacky, as this is not really for page tracking
	KVM_PAGE_TRACK_EXEC,
	KVM_PAGE_TRACK_RESET_EXEC,
	KVM_PAGE_TRACK_MAX,
};

typedef struct {
    // gpa of the page fault
    uint64_t faulted_gpa;
    sev_step_partial_vmcb_save_area_t decrypted_vmsa_data;
	/// @brief if true, decrypted_vmsa_data contains valid data
	bool is_decrypted_vmsa_data_valid;
} usp_page_fault_event_t;

typedef struct {
    uint32_t tmict_value;
    /// @brief May be null. If set, we reset the ACCESS bits of these pages before vmentry
	/// which improves single stepping accuracy
	uint64_t* gpas_target_pages;
	uint64_t gpas_target_pages_len;
} sev_step_param_t;

/**
 * @brief struct for storing sev-step event parameters
 * to send them to userspace
 */
typedef struct {
	uint32_t counted_instructions;
	sev_step_partial_vmcb_save_area_t decrypted_vmsa_data;
	/// @brief if true, decrypted_vmsa_data contains valid data
	bool is_decrypted_vmsa_data_valid;
	uint64_t* cache_attack_timings;
	uint64_t* cache_attack_perf_values;
	/// @brief length of both cache_attack_timings and
	/// cache_attack_perf_values
	uint64_t  cache_attack_data_len; 
} sev_step_event_t;

//
// Convenience Functions
//

void print_single_step_event(char* fp, sev_step_event_t* event);


//
// SEV-STEP Functions
//
/**
 * @brief
 *
 * @param ctx
 * @param got_event
 * @param event_type
 * @param event Callers must free with usp_free_event
 * @return int
 */
int usp_poll_event(usp_poll_api_ctx_t* ctx, int* got_event, usp_event_type_t* event_type, void** event);
void free_usp_event(usp_event_type_t type, void *event);
void usp_ack_event(usp_poll_api_ctx_t *ctx);
int usp_block_until_event(usp_poll_api_ctx_t* ctx, usp_event_type_t* event_type, void** event);
int get_size_for_event(usp_event_type_t event_type, uint64_t* size);
int track_page(usp_poll_api_ctx_t *ctx, uint64_t gpa, enum kvm_page_track_mode mode);
int untrack_page(usp_poll_api_ctx_t *ctx, uint64_t gpa, enum kvm_page_track_mode mode);
int track_all_pages(usp_poll_api_ctx_t *ctx, enum kvm_page_track_mode mode);
int untrack_all_pages(usp_poll_api_ctx_t *ctx, enum kvm_page_track_mode mode);
/**
 * @brief Open new api connection
 * 
 * @param ctx 
 * @param debug_mode If true, expect the VM to run in debug mode and instruct the kernel counterpart
 * to decrypt the VM's VMCB at each event and send the data with the event
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int usp_new_ctx(usp_poll_api_ctx_t* ctx, bool debug_mode);
int usp_close_ctx(usp_poll_api_ctx_t *ctx);
/**
 * @brief Enable single stepping upon next vmentry
 * 
 * @param ctx api ctx to operate on
 * @param timer_value timeout for apic timer
 * @param gpas_target_pages May be null. If set, we reset the ACCESS bits of these pages before vmentry
 * @param gpas_target_pages_len 
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int enable_single_stepping(usp_poll_api_ctx_t *ctx,uint32_t timer_value,uint64_t* gpas_target_pages,uint64_t gpas_target_pages_len);
int disable_single_stepping(usp_poll_api_ctx_t *ctx);

/**
 * @brief Inject an nmi into vm upon next vmentry
 * 
 * @param ctx 
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int inject_nmi(usp_poll_api_ctx_t* ctx);

/**
 * @brief  "Imports" an eviction set build in userspace by pinning the pages and creating mappings
 * to the underlying pages, so that we can use the eviction set anywhere in the kernel space.
 * Note: Does not work for Way predictor based eviction sets, as these depend on the virtual address, which
 * changes when we create our kernel space mapping (in constrast to the underlying physical page, which stays the same)
 * @param ctx 
 * @param params see struct definitions for docuementation. Don't free eviction set memory
 * until sev_step_free_evs was called
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int sev_step_import_user_evs(usp_poll_api_ctx_t* ctx, import_user_eviction_set_param_t* params);

/**
 * @brief Free the currently used eviction set in the kernel (if there is any)
 * 
 * @param ctx 
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int sev_step_free_evs(usp_poll_api_ctx_t* ctx);

/**
 * @brief Request kernel to build aliasing eviction set for load + reload attack from 
 * "Take A Way" paper
 * 
 * @param ctx 
 * @param build_ev_req params for cache attack
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int sev_step_build_alias_evs(usp_poll_api_ctx_t* ctx,build_eviction_set_param_t* build_ev_req);

/**
 * @brief Perform a cache attack on the next (single) step. Eviction set must already be loaded.
 * Result will be part of the step event
 * 
 * @param ctx 
 * @param lookup_table_idx idx of lookup table to attack
 * @param use_custom_apic_timer_value if, true, custom_apic_timer_value will be used for apic timer
 * when performing this cache attack
 * @param custom_apic_timer_value if use_custom_apic_timer_value is set, use this value for the apic timer
 * for the cache attack
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int sev_step_do_cache_attack_next_step(usp_poll_api_ctx_t* ctx, uint64_t lookup_table_idx,
    bool use_custom_apic_timer_value, uint32_t custom_apic_timer_value);

/**
 * @brief Translate GPA to HPA. This is e.g. required for eviction sets that are based
 * on phyiscal addresses
 * 
 * @param ctx 
 * @param gpa we want the host physical address for this guest physical address
 * @param hpa Result param
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int sev_step_gpa_to_hpa(usp_poll_api_ctx_t* ctx, uint64_t gpa, uint64_t *hpa);


int sev_step_cache_attack_testbed(usp_poll_api_ctx_t* ctx);

//
//Helper functions
//
char* tracking_mode_to_string(enum kvm_page_track_mode mode);

//
// SEV-STEP IOCTLs
// See sev-step-host-kernel/include/uapi/linux/kvm.h for documentation
//
#define KVM_TRACK_PAGE _IOWR(KVMIO, 0xb, track_page_param_t)
#define KVM_TRACK_ALL_PAGES _IOWR(KVMIO, 0xc, track_all_pages_t)
#define KVM_UNTRACK_ALL_PAGES _IOWR(KVMIO, 0xd, track_all_pages_t)
#define KVM_UNTRACK_PAGE _IOWR(KVMIO, 0xe, track_page_param_t)
#define KVM_USP_INIT_POLL_API _IOWR(KVMIO, 0xf, usp_init_poll_api_t)
#define KVM_USP_CLOSE_POLL_API _IO(KVMIO, 0x10)
#define KVM_SEV_STEP_ENABLE _IOWR(KVMIO, 0x11, sev_step_param_t)
#define KVM_SEV_STEP_DISABLE _IO(KVMIO, 0x12)
#define KVM_SEV_STEP_INJECT_NMI _IO(KVMIO, 0x13)
#define KVM_SEV_STEP_BUILD_EVS _IOWR(KVMIO, 0x15, build_eviction_set_param_t)
#define KVM_SEV_STEP_FREE_EVS _IO(KVMIO, 0x16)
#define KVM_SET_STEP_IMPORT_USER_EVS _IOWR(KVMIO, 0x17, import_user_eviction_set_param_t)
#define KVM_SEV_STEP_DO_CACHE_ATTACK_NEXT_STEP _IOWR(KVMIO, 0x18, do_cache_attack_param_t)
#define KVM_SEV_STEP_GPA_TO_HPA _IOWR(KVMIO, 0x19, gpa_to_hpa_param_t)
#define KVM_SEV_STEP_CACHE_ATTACK_TESTBED _IO(KVMIO, 0x20)
#define KVM_SEV_STEP_BUILD_ALIAS_EVS _IOWR(KVMIO, 0x21, build_eviction_set_param_t )

#endif