<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd"><html xmlns="http://www.w3.org/1999/xhtml"><!-- InstanceBegin template="/Templates/Main.dwt" codeOutsideHTMLIsLocked="false" -->
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <link rel="shortcut icon" href="images/favicon.ico" >
    <!-- InstanceBeginEditable name="doctitle" -->
    <title>Nektar++ - Downloads</title>
    <!-- InstanceEndEditable -->
    <style type="text/css">
    <!--
	html {
	  min-height: 101%; // force a scrollbar to always appear
	}
    body {
      font: 100% Verdana, Arial, Helvetica, sans-serif;
      background: #FFFFFF;
      margin: 0;
	  padding: 0;
	  text-align: center;
	  color: #000000;
    }
    .twoColHybLt #container {
	  //max-width: 90em;
	  max-width: 60em;
	  width: 99%;
      background: #FFFFFF;
      margin: 0 auto;
      text-align: left;
    }
    .twoColHybLt #sidebar1 {
	float: left;
	width: 13.7em;
	background: #EBEBEB;
	border: 1px solid #000000;
	padding-top: 15px;
	padding-right: 0;
	padding-bottom: 15px;
	padding-left: 0;
    }
    .twoColHybLt #sidebar1 h3, .twoColHybLt #sidebar1 p {
      margin-left: 10px;
      margin-right: 10px;
    }
    .twoColHybLt #mainContent {
	margin-top: 0;
	margin-right: 20px;
	margin-bottom: 0;
	margin-left: 14.5em;
    }
    .fltrt {
      float: right;
      margin-left: 8px;
    }
    .fltlft {
      float: left;
      margin-right: 8px;
    }
    .clearfloat {
      clear:both;
      height:0;
      font-size: 1px;
      line-height: 0px;
    }
    img {
      border:0;
    }
	h1,h2,h3 {
	  margin-bottom: 0;
	}
    -->
    </style>
    <!-- InstanceBeginEditable name="CSS_Style" -->
    <style type="text/css">
    <!--
    #menu_Downloads {
      font-weight: bold;
    }
    -->
    </style>
    <!-- InstanceEndEditable -->
    <!--[if IE]>
    <style type="text/css"> 
      .twoColHybLt #sidebar1 { padding-top: 30px; margin-top: 20px; }
      .twoColHybLt #mainContent { zoom: 1; padding-top: 15px; }
    </style>
    <![endif]-->
    <!-- InstanceParam name="LibraryMenu" type="boolean" value="false" -->
    <!-- InstanceParam name="CompileMenu" type="boolean" value="false" -->
  </head>
  <body class="twoColHybLt">
    <div id="container">
      <div id="sidebar1">
        <h3 style="text-align: center;"><a href="index.html" id="menu_Nektar"><img src="images/nektar.png" alt="Nektar++" width="155" height="38" /></a></h3>
        <p><a href="downloads.html" id="menu_Downloads">Downloads</a></p>
        <p>
          <a href="compile.html" id="menu_Compile">Compile Instructions</a>
    	  
        </p>
        <p>
          <a href="library.html" id="menu_Library">Library Overview</a>
    	  
        </p>
        <p><a href="team.html" id="menu_Team">Team Members</a></p>
<!--        <p><a href="pre-processing.html" id="menu_PreProcessing">Pre-processing</a></p>-->
<!--        <p><a href="solvers.html" id="menu_Solvers">Solvers</a></p>-->
        <p><a href="post-processing.html" id="menu_PostProcessing">Post-processing</a></p>
<!--        <p><a href="demos.html" id="menu_Demos">Demos</a></p>-->
        <p><a href="http://www.nektar.info/2nd_edition/" target="_blank" id="menu_Book2ndEd">Spectral/hp element methods for CFD</a> (Karniadakis and Sherwin)</p>
        <p><a href="educational_material.html" id="menu_EducationalMaterial">Educational Material</a></p>
        <p><a href="license.html" id="menu_License">License</a></p>
        <p><a href="acknowledgments.html" id="menu_Acknowledgments">Acknowledgments</a></p>
        <p><a href="mailto:nektar-inquiry@sci.utah.edu" id="menu_Contact">Contact</a></p>
        <p style="font-size:9px;">Last Revision:<br/>$Date: 2008/06/05 21:31:53 $</p>
      </div>
      <div id="mainContent">
        <!-- InstanceBeginEditable name="Title" -->
        <h1>Downloads</h1>
    	<!-- InstanceEndEditable -->
        <hr/>
        <!-- InstanceBeginEditable name="BodyText" -->
	    <p><a href="file.tar.gz">Nektar++ Unix/Windows Code</a></p>
    	<p><a href="file.tar.gz">Required Third Party Libraries</a></p>
	    <p><a href="getting_started.html" target="_blank">Getting Started Tutorial</a></p>
        <!-- InstanceEndEditable -->
        <p><br class="clearfloat" /></p>
      </div>
    </div>
  </body>
<!-- InstanceEnd --></html>
<!--
<?php
// Load connection constants.
require('common.php');

// Connect to MySQL.
$link = mysql_connect($hostname, $username, $password);
if (!$link) exit(1);

// Select the correct database.
$db_selected = mysql_select_db($database, $link);
if (!$db_selected) exit(1);

// Add the person to the database.
$query = sprintf("INSERT INTO `$table` (`Name`, `Email`, `Affiliation`) VALUES ('%s', '%s', '%s')",
                 mysql_real_escape_string($_GET["Name"], $link),
                 mysql_real_escape_string($_GET["Email"], $link),
                 mysql_real_escape_string($_GET["Affiliation"], $link));
mysql_query($query, $link);

// Close the connection to MySQL.
mysql_close($link);
?>
-->