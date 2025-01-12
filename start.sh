#!/bin/bash
cd build;rm -rf *;cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=deploy;cmake --build . --config Release; cmake --install . --prefix deploy --verbose;

