/*
 * Copyright (C) 2020 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cstdlib>
#include <cstring>
#include <sys/sysinfo.h>
#include <vector>

#include <android-base/properties.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include "libinit_variant.h"
#include "vendor_init.h"
#include "property_service.h"

using android::base::GetProperty;

std::vector<std::string> ro_props_default_source_order = {
    "",
    "odm.",
    "product.",
    "system.",
    "vendor.",
    "system_ext.",
};

void property_override(char const prop[], char const value[], bool add)
{
    auto pi = (prop_info *) __system_property_find(prop);

    if (pi != nullptr) {
        __system_property_update(pi, value, strlen(value));
    } else if (add) {
        __system_property_add(prop, strlen(prop), value, strlen(value));
    }
}

void set_ro_build_prop(const std::string &source, const std::string &prop,
        const std::string &value, bool product = false) {
    std::string prop_name;

    if (product) {
        prop_name = "ro.product." + source + prop;
    } else {
        prop_name = "ro." + source + "build." + prop;
    }

    property_override(prop_name.c_str(), value.c_str(), false);
}

void set_device_props(const std::string fingerprint, const std::string description,
        const std::string brand, const std::string device, const std::string model) {
    for (const auto &source : ro_props_default_source_order) {
        set_ro_build_prop(source, "fingerprint", fingerprint);
        set_ro_build_prop(source, "brand", brand, true);
        set_ro_build_prop(source, "device", device, true);
        set_ro_build_prop(source, "model", model, true);
    }

    property_override("ro.build.fingerprint", fingerprint.c_str());
    property_override("ro.build.description", description.c_str());
}

void load_device_properties() {
    std::string hwname = GetProperty("ro.boot.hwname", "");
    std::string region = GetProperty("ro.boot.hwc", "");

    if (hwname == "curtana") {
        if (region == "Global_TWO") {
            set_device_props(
                    "Redmi/curtana_global/curtana:12/RKQ1.211019.001/V14.0.4.0.SJWMIXM:user/release-keys",
                    "curtana_global-user 12 RKQ1.211019.001 V14.0.4.0.SJWMIXM release-keys",
                    "Redmi", "curtana", "Redmi Note 9S");
            property_override("ro.product.mod_device", "curtana_global");

        } else if (region == "Global_PA") {
            set_device_props(
                    "Redmi/curtana_global/curtana:12/RKQ1.211019.001/V14.0.4.0.SJWMIXM:user/release-keys",
                    "curtana_global-user 12 RKQ1.211019.001 V14.0.4.0.SJWMIXM release-keys",
                    "Redmi", "curtana", "Redmi Note 9S");
            property_override("ro.product.mod_device", "curtana_global");

        } else if (region == "India") {
            set_device_props(
                    "Redmi/curtana_in1/curtana:12/RKQ1.211019.001/V14.0.5.0.SJWINRF:user/release-keys",
                    "curtana_in1-user 12 RKQ1.211019.001 V14.0.5.0.SJWINRF release-keys",
                    "Redmi", "curtana", "Redmi Note 9 Pro");
            property_override("ro.product.mod_device", "curtana_in_global");
        } else if (region == "Japan") {
            set_device_props(
                    "Redmi/curtana_global/curtana:12/RKQ1.211019.001/V14.0.4.0.SJWMIXM:user/release-keys",
                    "curtana_global-user 12 RKQ1.211019.001 V14.0.4.0.SJWMIXM release-keys",
                    "Redmi", "curtana", "Redmi Note 9S");
            property_override("ro.product.mod_device", "curtana_global");
        }
    } else if (hwname == "excalibur") {
        set_device_props(
                "Redmi/excalibur_in/excalibur:12/RKQ1.211019.001/V14.0.2.0.SJXINXM:user/release-keys",
                "excalibur_in-user 12 RKQ1.211019.001 V14.0.2.0.SJXINXM release-keys",
                "Redmi", "excalibur", "Redmi Note 9 Pro Max");
        property_override("ro.product.mod_device", "excalibur_in_global");
    } else if (hwname == "gram") {
        set_device_props(
                "POCO/gram_in/gram:12/RKQ1.211019.001/V14.0.5.0.SJPINXM:user/release-keys",
                "gram_in-user 12 RKQ1.211019.001 V14.0.5.0.SJPINXM release-keys",
                "POCO", "gram", "POCO M2 Pro");
        property_override("ro.product.mod_device", "gram_in_global");
    } else if (hwname == "joyeuse") {
        set_device_props(
                "Redmi/joyeuse_global/joyeuse:12/RKQ1.211019.001/V14.0.3.0.SJZMIXM:user/release-keys",
                "joyeuse_global-user 12 RKQ1.211019.001 V14.0.3.0.SJZMIXM release-keys",
                "Redmi", "joyeuse", "Redmi Note 9 Pro");
        property_override("ro.product.mod_device", "joyeuse_global");
        property_override("ro.boot.product.hardware.sku", "joyeuse");
    }
}

// dalvik_heap
void load_dalvik_properties() {
    char const *heapstartsize;
    char const *heapgrowthlimit;
    char const *heapsize;
    char const *heapminfree;
    char const *heapmaxfree;
    char const *heaptargetutilization;
    struct sysinfo sys;

    sysinfo(&sys);

    if (sys.totalram >= 5ull * 1024 * 1024 * 1024) {
        // from - phone-xhdpi-6144-dalvik-heap.mk
        heapstartsize = "16m";
        heapgrowthlimit = "256m";
        heapsize = "512m";
        heaptargetutilization = "0.5";
        heapminfree = "8m";
        heapmaxfree = "32m";
    } else if (sys.totalram >= 3ull * 1024 * 1024 * 1024) {
        // from - phone-xhdpi-4096-dalvik-heap.mk
        heapstartsize = "8m";
        heapgrowthlimit = "192m";
        heapsize = "512m";
        heaptargetutilization = "0.6";
        heapminfree = "8m";
        heapmaxfree = "16m";
    } else {
        return;
    }

    property_override("dalvik.vm.heapstartsize", heapstartsize);
    property_override("dalvik.vm.heapgrowthlimit", heapgrowthlimit);
    property_override("dalvik.vm.heapsize", heapsize);
    property_override("dalvik.vm.heaptargetutilization", heaptargetutilization);
    property_override("dalvik.vm.heapminfree", heapminfree);
    property_override("dalvik.vm.heapmaxfree", heapmaxfree);
}

void vendor_load_properties() {
    load_device_properties();
    load_dalvik_properties();
}