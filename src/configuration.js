function Configuration() {
  this.CONFIG_STORE = 'configv2';
  this.DEFAULT_CONFIG = {
    defaultView: 0,
    animationTransitionTime: 700,
    filterMostRelevamtNodes: 700,
    normalGradiante: [
    {
     color: "green",
     position: 1
   },
   {
     color: "yellow",
     position: 50
   },
   {
     color: "rgba(255,0,0,1)",
     position: 99
   }
   ],
   scriptsLoad: ["data_ex.js"],
   scriptsLoadAutoComplete: ["data_ex.js", "data_ex2.js"],
   regexFilter: null
 };

 var self = this;

 this.saveConfig = function() {
  localStorage.setItem(this.CONFIG_STORE, JSON.stringify(self.currentConfig));
}

this.saveGradiante = function() {
  self.saveConfig();
  self.gradiante = new Gradiant(self.currentConfig.normalGradiante);
}

this.resetConfig = function(){
  self.currentConfig = jQuery.extend(this.DEFAULT_CONFIG,  {
    lastViewed: this.DEFAULT_CONFIG.defaultView
  });
  self.saveGradiante();
}

var localStorageConf = localStorage.getItem(this.CONFIG_STORE);
if (localStorageConf == null || !(localStorageConf = JSON.parse(localStorageConf))) {
  self.resetConfig();
}
else {
  this.currentConfig = localStorageConf;
}

}