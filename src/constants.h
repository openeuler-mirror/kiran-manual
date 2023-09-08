/**
 * Copyright (c) 2020 ~ 2024 KylinSec Co., Ltd.
 * kiran-manual is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 *
 * Author:     youzhengcai <youzhengcai@kylinsec.com.cn>
 */

#pragma once

#include "config.h"

// Project config
#define PROJECT_NAME "kiran-manual"
//#define CONF_FILE_PATH ":/data/km.ini";
// Project Data File Path
#define ROOT_FOLDER INSTALL_DATADIR "/kiran-manual/"
#define DATA_FOLDER ROOT_FOLDER "data/"
#define DOCUMENTS_FOLDER DATA_FOLDER "documents/"
#define MARKDOWNS_FOLDER DOCUMENTS_FOLDER "markdowns/"
#define IMAGE_FOLDER DOCUMENTS_FOLDER "images/"
#define IMAGE_FOR_DOCS_FOLDER IMAGE_FOLDER "docs/"
#define IMAGE_FOR_NAV_FOLDER IMAGE_FOLDER "nav/"
#define CONF_FILE_PATH DATA_FOLDER "km.ini";

// Resources
#define RESOURCES_FOLDER ":/resources/"
#define ICON_PATH RESOURCES_FOLDER "images/kiran-manual.png"

// UI
#define WINDOW_WIDTH 1035
#define WINDOW_HEIGHT 742
#define TITLE_BAR_HEIGHT 40

// Object name
#define NAVIGATION_OBJECT_NAME "Navigation"
#define DOCUMENT_OBJECT_NAME "Document"