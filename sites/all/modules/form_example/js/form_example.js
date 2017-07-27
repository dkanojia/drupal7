// // Using the closure to map jQuery to $. 
// (function ($) {

// // Store our function as a property of Drupal.behaviors.
// Drupal.behaviors.myModuleSecureLink = {
//   attach: function (context, settings) {
//     // Find all the secure links inside context that do not have our processed
//     // class.
//     $('a[href^="https://"]', context)
//       // Only process elements once.
//       .once('secureLink')
//       // Then stick some text into the link denoting it as secure.
//       .append(' (Secure!)');
//   }
// };

// // You could add additional behaviors here.
// Drupal.behaviors.myModuleMagic = {
//   attach: function (context, settings) { },
//   detach: function (context, settings) { }
// };

// }(jQuery));
(function($) {
Drupal.behaviors.myBehavior = {
  attach: function (context, settings) {

    //code starts
    $("body").click(function() {
      // alert("Welcome to First Js File");
    });
    //code ends

  }
};
})(jQuery);
