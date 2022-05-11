#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright 2022 Alibaba Group Holding Limited. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import glob
import itertools
import os
import sys
import zipfile

DEFAULT_GS_CONFIG_FILE = ".gs_conf.yaml"


def package_app(app_dir):
    app_name = os.path.basename(app_dir)
    with zipfile.ZipFile(app_name + ".gar", "w") as gar:
        for filename in itertools.chain(
            glob.glob(os.path.join(app_dir, "*.h")),
            glob.glob(os.path.join(app_dir, "*.cc")),
        ):
            gar.write(filename, os.path.basename(filename))
        gar.write(os.path.join(app_dir, "conf.yaml"), DEFAULT_GS_CONFIG_FILE)


if __name__ == "__main__":
    package_app(sys.argv[1])
