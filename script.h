const char script[] PROGMEM = R"rawLiteral(
  var Socket;
  function connect() {
  Socket = new WebSocket('ws://' + window.location.hostname + ':8080/');
  Socket.onopen = function() {
  };
  Socket.onmessage = function(event) {
      processCommand(event);
  };
  Socket.onclose = function(e) {
    console.log('Socket is closed. Reconnect will be attempted in 1 second.', e.reason);
    setTimeout(function() {
      connect();
    }, 1000);
  };
  Socket.onerror = function(err) {
    console.error('Socket encountered error: ', err.message, 'Closing socket');
    Socket.close();
  };
}
  async function init() {
    connect();
    document.getElementById('saveButton').addEventListener('click', saveButton);
    document.getElementById('resetButton').addEventListener('click', resetButton);
    document.getElementById('dhcpButton').addEventListener('click', dhcpButton);
    getLogo();
  }
    const url = '/logo'
    const getImage = async (url) => {
       return await fetch(url).then(res => res.text())
    }
    function getLogo(){
    getImage(url)
      .then(result => {
        document.getElementById('logoImage').src = result
      })
    }; 
)rawLiteral"; 
