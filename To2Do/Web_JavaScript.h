#ifndef WEB_JAVASCRIPT_H
#define WEB_JAVASCRIPT_H

#include <Arduino.h>
#include "Web_JavaScript_UI.h"
#include "Web_JavaScript_Actions.h"
#include "Web_JavaScript_Core.h"

// Ana JavaScript dosyasi - 3 kutuphaneyi birlestirir
// 1. UI - Arayuz render fonksiyonlari
// 2. Actions - Butonlar ve popup'lar
// 3. Core - Veri yonetimi ve server iletisimi
String getJavaScriptCombined() {
  static bool initialized = false;
  static String combinedJS = "";
  
  if (!initialized) {
    combinedJS.reserve(32000); // Memory optimization
    combinedJS += getJavaScriptUI();
    combinedJS += getJavaScriptActions();
    combinedJS += getJavaScriptCore();
    initialized = true;
  }
  
  return combinedJS;
}

// WARNING: This function returns a reference to a static String.
// The String persists in memory for the lifetime of the program.
// Use this only for content that needs to be served repeatedly.
const String& getJavaScript() {
  static const String js = getJavaScriptCombined();
  return js;
}

// Alternative if you need to avoid static storage entirely:
// const char* getJavaScriptOnce() {
//   static bool called = false;
//   static String js = "";
//   if (!called) {
//     js = getJavaScriptCombined();
//     called = true;
//   }
//   return js.c_str();
// }

#endif
