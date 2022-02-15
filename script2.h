const char script2[] PROGMEM =R"rawLiteral(
function saveButton() {
    var obj = {
      protocol : document.getElementById("protocol").innerHTML,
      rtrIPaddr : document.getElementById("rtrIPaddr").value,
      rtrPort : parseInt(document.getElementById("rtrPort").value),
      ipAddr : document.getElementById("ipAddr").value,
      subnet : document.getElementById("subnet").value,
      gw : document.getElementById("gw").value,
      dhcp : document.getElementById("dhcpButton").checked,
    };
    Socket.send(JSON.stringify(obj));
    console.log(obj);
      var msg = {
      save: true
  };
  Socket.send(JSON.stringify(msg));
  }
  function resetButton() {
    var msg = {
      reset: true
  };
  Socket.send(JSON.stringify(msg));
  console.log("Reboot...");
  }
   function dhcpButton() {
    var checked = document.getElementById("dhcpButton").checked;
      document.getElementById("ipAddr").disabled = checked;
      document.getElementById("subnet").disabled = checked;
      document.getElementById("gw").disabled = checked;
  console.log("dhcp...");
  }
  function stateFuncDn(evt) {
    evt.preventDefault();
    var msg = {
      tally: this.index,
      pushed: false
    };
    Socket.send(JSON.stringify(msg));
    console.log(msg);
  }
  function stateFuncUp(evt) { //same as above but might change later?
    evt.preventDefault();
    var msg = {
      tally: this.index,
      pushed: true
    };
    Socket.send(JSON.stringify(msg));
    console.log(msg);
  }
  function modeFunc() {
    var msg = {
      mode: this.index,
    };
    Socket.send(JSON.stringify(msg));
    console.log(msg);
  }
  function processCommand(event) {
  var obj = JSON.parse(event.data);
  console.log(obj);
  if(obj.protocol){
  document.getElementById('protocol').innerHTML = obj.protocol;
  }
  if(obj.rtrIPaddr){
   document.getElementById('rtrIPaddr').value = obj.rtrIPaddr;
  }
  if(obj.rtrPort){
   document.getElementById('rtrPort').value = obj.rtrPort;
  }
if(obj.ipAddr){
    document.getElementById('ipAddr').value = obj.ipAddr;
}
if(obj.subnet){
    document.getElementById('subnet').value = obj.subnet;
}
if(obj.gw){
    document.getElementById('gw').value = obj.gw;
}
if(obj.dhcp){
    document.getElementById('dhcpButton').checked = obj.dhcp;
    document.getElementById("ipAddr").disabled = obj.dhcp;
    document.getElementById("subnet").disabled = obj.dhcp;
    document.getElementById("gw").disabled = obj.dhcp;
}
 
  }
  window.onload = function(event) {
    init();
  }

  )rawLiteral"; 
