// --- CMAKE generated variables for your plugin

#include "pluginstructures.h"

#ifndef _plugindescription_h
#define _plugindescription_h

#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)
#define AU_COCOA_VIEWFACTORY_STRING STR(AU_COCOA_VIEWFACTORY_NAME)
#define AU_COCOA_VIEW_STRING STR(AU_COCOA_VIEW_NAME)

// --- AU Plugin Cocoa View Names (flat namespace)
#define AU_COCOA_VIEWFACTORY_NAME AUCocoaViewFactory_382D5308BC614C278F11896872264167
#define AU_COCOA_VIEW_NAME AUCocoaView_382D5308BC614C278F11896872264167

// --- BUNDLE IDs (MacOS Only)
const char* kAAXBundleID = "rackafxuser.aax.FFCompressor.bundleID";
const char* kAUBundleID = "rackafxuser.au.FFCompressor.bundleID";
const char* kVST3BundleID = "rackafxuser.vst3.FFCompressor.bundleID";

// --- Plugin Names
const char* kPluginName = "FFCompressor";
const char* kShortPluginName = "FFCompressor";
const char* kAUBundleName = "FFCompressor";
const char* kAAXBundleName = "FFCompressor";
const char* kVSTBundleName = "FFCompressor";

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
const char* kVSTFUID = "{382D5308-BC61-4C27-8F11-896872264167}";

// --- 4-char codes
const int32_t kFourCharCode = 'PN5a';
const int32_t kAAXProductID = 'jzSX';
const int32_t kManufacturerID = 'VNDR';

// --- Vendor information
const char* kVendorName = "RackAFX User";
const char* kVendorURL = "www.yourcompany.com";
const char* kVendorEmail = "help@yourcompany.com";

// --- Plugin Options
const bool kProcessFrames = true;
const uint32_t kBlockSize = DEFAULT_AUDIO_BLOCK_SIZE;
const bool kWantSidechain = false;
const uint32_t kLatencyInSamples = 0;
const double kTailTimeMsec = 0.000;
const bool kVSTInfiniteTail = false;
const bool kVSTSAA = false;
const uint32_t kVST3SAAGranularity = 1;
const uint32_t kAAXCategory = 0;

#endif


