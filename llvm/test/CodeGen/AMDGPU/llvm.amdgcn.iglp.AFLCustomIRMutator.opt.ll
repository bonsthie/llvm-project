; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py UTC_ARGS: --version 5
; RUN: llc -mtriple=amdgcn-amd-amdhsa -mcpu=gfx90a -O1 < %s | FileCheck -check-prefix=GCN %s

define amdgpu_kernel void @test_iglp_opt_rev_mfma_gemm(<1 x i64> %L1) {
; GCN-LABEL: test_iglp_opt_rev_mfma_gemm:
; GCN:       ; %bb.0: ; %entry
; GCN-NEXT:    v_mov_b32_e32 v32, 0
; GCN-NEXT:    ds_read_b128 v[0:3], v32
; GCN-NEXT:    s_load_dwordx2 s[0:1], s[8:9], 0x0
; GCN-NEXT:    ds_read_b128 v[28:31], v32 offset:112
; GCN-NEXT:    ds_read_b128 v[24:27], v32 offset:96
; GCN-NEXT:    ds_read_b128 v[20:23], v32 offset:80
; GCN-NEXT:    ds_read_b128 v[16:19], v32 offset:64
; GCN-NEXT:    ds_read_b128 v[4:7], v32 offset:16
; GCN-NEXT:    ds_read_b128 v[8:11], v32 offset:32
; GCN-NEXT:    ds_read_b128 v[12:15], v32 offset:48
; GCN-NEXT:    s_waitcnt lgkmcnt(0)
; GCN-NEXT:    ds_write_b128 v32, v[0:3]
; GCN-NEXT:    v_mov_b32_e32 v0, 0
; GCN-NEXT:    v_mov_b32_e32 v1, v0
; GCN-NEXT:    s_cmp_lg_u64 s[0:1], 0
; GCN-NEXT:    ; iglp_opt mask(0x00000001)
; GCN-NEXT:    ds_write_b128 v32, v[28:31] offset:112
; GCN-NEXT:    ds_write_b128 v32, v[24:27] offset:96
; GCN-NEXT:    ds_write_b128 v32, v[20:23] offset:80
; GCN-NEXT:    ds_write_b128 v32, v[16:19] offset:64
; GCN-NEXT:    ds_write_b128 v32, v[12:15] offset:48
; GCN-NEXT:    ds_write_b128 v32, v[8:11] offset:32
; GCN-NEXT:    ds_write_b128 v32, v[4:7] offset:16
; GCN-NEXT:    ds_write_b64 v32, v[0:1]
; GCN-NEXT:    s_endpgm
entry:
  call void @llvm.amdgcn.iglp.opt(i32 1)
  %load.4 = load <32 x float>, ptr addrspace(3) null, align 128
  %B = urem <1 x i64> zeroinitializer, %L1
  store <32 x float> %load.4, ptr addrspace(3) null, align 128
  store <1 x i64> %B, ptr addrspace(3) null, align 8
  ret void
}

declare void @llvm.amdgcn.iglp.opt(i32 immarg) #0

attributes #0 = { convergent nocallback nofree nounwind willreturn }
