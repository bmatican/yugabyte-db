// Copyright (c) YugaByte, Inc.

#include "yb/master/master.h"

#include "yb/master/master_backup.service.h"
#include "yb/master/master_backup_service.h"

#include "yb/rpc/secure_stream.h"

#include "yb/server/hybrid_clock.h"
#include "yb/server/secure.h"

#include "yb/util/flag_tags.h"
#include "yb/util/ntp_clock.h"

DEFINE_int32(master_backup_svc_queue_length, 50,
             "RPC queue length for master backup service");
TAG_FLAG(master_backup_svc_queue_length, advanced);

namespace yb {
namespace master {
namespace enterprise {

using yb::rpc::ServiceIf;

Master::Master(const MasterOptions& opts) : super(opts) {
}

Master::~Master() {
}

Status Master::RegisterServices() {
#if !defined(__APPLE__)
  server::HybridClock::RegisterProvider(NtpClock::Name(), [] {
    return std::make_shared<NtpClock>();
  });
#endif

  std::unique_ptr<ServiceIf> master_backup_service(new MasterBackupServiceImpl(this));
  RETURN_NOT_OK(RpcAndWebServerBase::RegisterService(FLAGS_master_backup_svc_queue_length,
                                                     std::move(master_backup_service)));

  return super::RegisterServices();
}

Status Master::SetupMessengerBuilder(rpc::MessengerBuilder* builder) {
  RETURN_NOT_OK(super::SetupMessengerBuilder(builder));
  secure_context_ = VERIFY_RESULT(server::SetupSecureContext(
      options_.rpc_opts.rpc_bind_addresses, fs_manager_.get(),
      server::SecureContextType::kServerToServer, builder));
  return Status::OK();
}

} // namespace enterprise
} // namespace master
} // namespace yb