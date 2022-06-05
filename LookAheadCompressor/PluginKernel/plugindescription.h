// --- CMAKE generated variables for your plugin

#include "pluginstructures.h"

#ifndef _plugindescription_h
#define _plugindescription_h

#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)
#define AU_COCOA_VIEWFACTORY_STRING STR(AU_COCOA_VIEWFACTORY_NAME)
#define AU_COCOA_VIEW_STRING STR(AU_COCOA_VIEW_NAME)

// --- AU Plugin Cocoa View Names (flat namespace)
#define AU_COCOA_VIEWFACTORY_NAME AUCocoaViewFactory_6E7E8DF4308F45CCBF2070FF8F6E6695
#define AU_COCOA_VIEW_NAME AUCocoaView_6E7E8DF4308F45CCBF2070FF8F6E6695

// --- BUNDLE IDs (MacOS Only)
const char* kAAXBundleID = "rackafxuser.aax.LookAheadCompressor.bundleID";
const char* kAUBundleID = "rackafxuser.au.LookAheadCompressor.bundleID";
const char* kVST3BundleID = "rackafxuser.vst3.LookAheadCompressor.bundleID";

// --- Plugin Names
const char* kPluginName = "LookAheadCompressor";
const char* kShortPluginName = "LookAheadCompre";
const char* kAUBundleName = "LookAheadCompressor";
const char* kAAXBundleName = "LookAheadCompressor";
const char* kVSTBundleName = "LookAheadCompressor";

// --- bundle name helper
inline static const char* getPluginDescBundleName()
{
#ifdef AUPLUGIN
	return kAUBundleName;
#endif

#ifdef AAXPLUGIN
	return kAAXBundleName;
#endif

#ifdef VSTPLUGIN
	return kVSTBundleName;
#endif

	// --- should never get here
	return kPluginName;
}

// --- Plugin Type
const pluginType kPluginType = pluginType::kFXPlugin;

// --- VST3 UUID
const char* kVSTFUID = "{6E7E8DF4-308F-45CC-BF20-70FF8F6E6695}";

// --- 4-char codes
const int32_t kFourCharCode = '8ZcS';
const int32_t kAAXProductID = 'MCD1';
const int32_t kManufacturerID = 'VNDR';

// --- Vendor information
const char* kVendorName = "RackAFX User";
const char* kVendorURL = "www.yourcompany.com";
const char* kVendorEmail = "help@yourcompany.com";

// --- Plugin Options
const bool kProcessFrames = true;
const uint32_t kBlockSize = DEFAULT_AUDIO_BLOCK_SIZE;
const bool kWantSidechain = true;
const uint32_t kLatencyInSamples = 0;
const double kTailTimeMsec = 0.000;
const bool kVSTInfiniteTail = false;
const bool kVSTSAA = false;
const uint32_t kVST3SAAGranularity = 1;
const uint32_t kAAXCategory = 0;

#endif


