# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

ARG org
ARG arch=amd64
FROM ${org}/${arch}-conda-cpp:latest

ARG arch=amd64
ARG maven=3.5
ARG node=11
ARG jdk=8
ARG go=1.12

RUN conda install -q \
        numpy \
        maven=${maven} \
        nodejs=${node} \
        openjdk=${jdk} && \
    conda clean --all

ENV GOROOT=/opt/go \
    GOBIN=/opt/go/bin \
    GOPATH=/go \
    PATH=/opt/go/bin:$PATH
RUN wget -nv -O - https://dl.google.com/go/go${go}.linux-${arch}.tar.gz | tar -xzf - -C /opt

ENV ARROW_BUILD_INTEGRATION=ON \
    ARROW_FLIGHT=ON \
    ARROW_ORC=OFF \
    ARROW_DATASET=OFF \
    ARROW_GANDIVA=OFF \
    ARROW_PLASMA=OFF \
    ARROW_FILESYSTEM=OFF \
    ARROW_HDFS=OFF \
    ARROW_JEMALLOC=OFF \
    ARROW_JSON=OFF \
    ARROW_USE_GLOG=OFF
