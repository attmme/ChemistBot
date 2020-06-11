<html>
	<head>
		<link rel="stylesheet" type="text/css" href="css/style.css">
		<link rel="shortcut icon" href="img/pill.ico" type="image/x-icon" >
		<title>Chemist Bot</title>
	</head>
		
	<body>	
		 <form id="formulari" method="POST" action="private/authen_login.php" >
			<img src="img/login.png" id="login_pic"/>
			 
            <table width="100%" border = 0>
                <tr>
                    <td> <input type="text" placeholder="Usuari" name="usuari"> </td>
				</tr>
				
				<tr>
                    <td> <input type="password" placeholder="Contrasenya" name="contrasenya"/> </td>
				</tr>
				
				<tr>
					<td> <input id="login_inp" class="login" type="submit" value="Login"/> </td>
                </tr>
            </table>
        </form>
		
		<?php
			// Mirem si està logejat
			session_start();
			if ($_SESSION['estat'] == 1){
				header("Location:perfil.php");
			}
		?>
	</body>
</html>