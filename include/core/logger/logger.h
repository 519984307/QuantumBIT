//
// Created by 11851 on 2023/4/7.
//
#include "spdlog/spdlog.h"

#ifndef LOGGER_H
#define LOGGER_H

void initialLogger(const char *logger_dir, const char *daily_file_name);

std::shared_ptr<spdlog::logger> getLogger(const char *name);

#endif //LOGGER_H
