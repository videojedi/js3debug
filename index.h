const char webpage[] PROGMEM =R"rawLiteral(
  <!DOCTYPE HTML><html>
    <head>
      <title>ethernet connection test</title>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <link rel="icon" href="data:,">
      <link rel="stylesheet" href="styles.css" media="print" onload="this.media='all'">
      <script src="script.js"></script>
      <script src="script2.js"></script>
      
    <title>ethernet connection test</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    </head>
    <body>
      <div class="topnav">
        <img class="logo" id="logoImage" src="" alt="logo"/>
        <h1>Ethernet test</h1>

     </div>
      <div class="content">
        <div class="card">
          <h2>Hardware config</h2>
          <div>
            <table class="RouterTable">
              <thead>
                <tr>
                  <th>Protocol</th>
                  <th>IP Address</th>
                  <th>Port #</th>
                </tr>
              </thead>
              <tbody>
                <tr>
                  <td id="protocol">----</td>
                  <td><input class="configIP" type="text" id="rtrIPaddr" maxlength="15" pattern="((^|\.)((25[0-5])|(2[0-4]\d)|(1\d\d)|([1-9]?\d))){4}$" oninput="this.reportValidity()" value = "----"></td>
                  <td><input class="configIP" type="number" pattern="\d*" oninput="this.reportValidity()" id="rtrPort" maxlength="5"  min="1" max="65535"  value = "----"></td>
                </tr>
              </tbody>
            </table>
          </div>
          <div>
    
          </div>
          
          <div>
            <h2>Interface Settings</h2>
            <table class="interfaceTable">
              <thead>
                <tr>
                  <th>DHCP?</th>
                  <th>IP Address</th>
                  <th>Subnet</th>
                  <th>Gateway</th>
                  
                </tr>
              </thead>
              <tbody>
                <tr>
                  <td><input id ="dhcpButton" type="checkbox"></td>
                  <td><input class="configIP" type="text" id="ipAddr" maxlength="15" pattern="((^|\.)((25[0-5])|(2[0-4]\d)|(1\d\d)|([1-9]?\d))){4}$" oninput="this.reportValidity()" value = "----"></td>
                  <td><input class="configIP" type="text" id="subnet" maxlength="15" pattern="((^|\.)((25[0-5])|(2[0-4]\d)|(1\d\d)|([1-9]?\d))){4}$" oninput="this.reportValidity()" value = "----"></td>
                  <td><input class="configIP" type="text" id="gw" maxlength="15" pattern="((^|\.)((25[0-5])|(2[0-4]\d)|(1\d\d)|([1-9]?\d))){4}$" oninput="this.reportValidity()" value = "----"></td>
                </tr>
              </tbody>
            </table>
          </div>
          <div> 
            <button id="saveButton" class="button">Save</button><button id="resetButton" class="button">Reboot</button>
          </div>
        </div>
      </div>

</body>
<footer>

</footer>

</html>
)rawLiteral";
