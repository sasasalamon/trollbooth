document.addEventListener('DOMContentLoaded', function() {
  /*initialize animations*/
  document.getElementsByTagName('body')[0].classList.add('is-loading');
  window.addEventListener('load', function() {
    window.setTimeout(function() {
      document.getElementsByTagName('body')[0].classList.remove('is-loading');
    }, 250);
  });
});
