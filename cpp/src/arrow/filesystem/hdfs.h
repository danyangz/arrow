// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "arrow/filesystem/filesystem.h"
#include "arrow/io/hdfs.h"
#include "arrow/result.h"
#include "arrow/util/uri.h"

namespace arrow {
namespace fs {

/// Options for the HDFS implementation.
struct ARROW_EXPORT HdfsOptions {
  HdfsOptions() = default;
  ~HdfsOptions() = default;

  /// Hdfs configuration options, contains host, port, driver
  io::HdfsConnectionConfig connection_config;

  /// Used by Hdfs OpenWritable Interface.
  int32_t buffer_size = 0;
  int16_t replication = 3;
  int64_t default_block_size = 0;

  void ConfigureEndPoint(const std::string& host, int port);
  /// Be cautious that libhdfs3 is a unmaintained project
  void ConfigureHdfsDriver(bool use_hdfs3);
  void ConfigureHdfsReplication(int16_t replication);
  void ConfigureHdfsUser(const std::string& user_name);
  void ConfigureHdfsBufferSize(int32_t buffer_size);
  void ConfigureHdfsBlockSize(int64_t default_block_size);

  static Result<HdfsOptions> FromUri(const ::arrow::internal::Uri& uri);
};

/// HDFS-backed FileSystem implementation.
///
/// implementation notes:
/// - This is a wrapper of arrow/io/hdfs, so we can use FileSystem API to handle hdfs.
class ARROW_EXPORT HadoopFileSystem : public FileSystem {
 public:
  ~HadoopFileSystem() override;

  /// \cond FALSE
  using FileSystem::GetTargetStats;
  /// \endcond
  Status GetTargetStats(const std::string& path, FileStats* out) override;
  Status GetTargetStats(const Selector& select, std::vector<FileStats>* out) override;

  Status CreateDir(const std::string& path, bool recursive = true) override;

  Status DeleteDir(const std::string& path) override;

  Status DeleteDirContents(const std::string& path) override;

  Status DeleteFile(const std::string& path) override;

  Status Move(const std::string& src, const std::string& dest) override;

  Status CopyFile(const std::string& src, const std::string& dest) override;

  Status OpenInputStream(const std::string& path,
                         std::shared_ptr<io::InputStream>* out) override;

  Status OpenInputFile(const std::string& path,
                       std::shared_ptr<io::RandomAccessFile>* out) override;

  Status OpenOutputStream(const std::string& path,
                          std::shared_ptr<io::OutputStream>* out) override;

  Status OpenAppendStream(const std::string& path,
                          std::shared_ptr<io::OutputStream>* out) override;

  /// Create a HdfsFileSystem instance from the given options.
  static Result<std::shared_ptr<HadoopFileSystem>> Make(const HdfsOptions& options);

 protected:
  explicit HadoopFileSystem(const HdfsOptions& options);

  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace fs
}  // namespace arrow
