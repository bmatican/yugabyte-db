// Copyright (c) YugaByte, Inc.

#ifndef ENT_SRC_YB_MASTER_MASTER_BACKUP_SERVICE_H
#define ENT_SRC_YB_MASTER_MASTER_BACKUP_SERVICE_H

#include "yb/master/master_backup.service.h"
#include "yb/master/master_service_base.h"

namespace yb {
namespace master {

// Implementation of the master backup service. See master_backup.proto.
class MasterBackupServiceImpl : public MasterBackupServiceIf,
                                public MasterServiceBase {
 public:
  explicit MasterBackupServiceImpl(Master* server);

  virtual void CreateSnapshot(
      const CreateSnapshotRequestPB* req, CreateSnapshotResponsePB* resp,
      rpc::RpcContext rpc) override;

  virtual void IsCreateSnapshotDone(
      const IsCreateSnapshotDoneRequestPB* req, IsCreateSnapshotDoneResponsePB* resp,
      rpc::RpcContext rpc) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(MasterBackupServiceImpl);
};

} // namespace master
} // namespace yb

#endif // ENT_SRC_YB_MASTER_MASTER_BACKUP_SERVICE_H
