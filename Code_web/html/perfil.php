<html>
	<head>
		<link rel="stylesheet" type="text/css" href="css/style.css">
		<link rel="shortcut icon" href="img/pill.ico" type="image/x-icon" >
		<title>Chemist Bot</title>
	</head>
	
	<header></header>
	
	<body>
		
		<nav>
			<ul>
				<li><a href="medicament.php">Medicaments</a></li>
				<li><a href="dispensar.php">Dispensar</a></li>
				<li><a href="horaris.php">Horaris programats</a></li>
				<li><a href="perfil.php" class="actual">Perfil</a></li>
			</ul>
		</nav>
		
		<!-- VISTA - PERFIL -->
		<table width=100% height=92% cellspacing=0 border=0	>
			<?php
				include_once 'private/db.php';
			
				// Mirem si està logejat
				session_start();

				if ($_SESSION['estat'] != 1){
					header("Location:index.php");
				}
			
				// Agafem les dades de l'usuari de la BDD
				$query = mysqli_query($conn, "SELECT usuari, nom, coalesce(pes, 0) pes, 
					coalesce(altura, 0) altura, coalesce(edat, 0) edat FROM usuaris WHERE id = " . $_SESSION['id_usuari']);
				$row = $conn->query($query);
				$contador = $row->num_rows;

				// Variables
				$usuari; $nom;
				$pes; $altura; 
				$edat;
				$imc = 0;

				// Imprimim les dades
				if (mysqli_num_rows($query) > 0) {
					while($row = mysqli_fetch_array($query)) {
						$usuari = mysqli_real_escape_string($conn,$row['usuari']);
						$nom = mysqli_real_escape_string($conn, $row['nom']);
						$pes = mysqli_real_escape_string($conn, $row['pes']);
						$altura = mysqli_real_escape_string($conn, $row['altura']); 
						$edat = mysqli_real_escape_string($conn, $row['edat']);
						
						// Calcul IMC
						if ($altura != 0 || $pes != 0){
							$imc = round($pes / ($altura * $altura), 2);
						}	
					}
				}
			?>
			
			<tr>
				<td class="imparell" width = 50% height = 45%>
					<table width=40%>
						<form action="#" method="post"> 
							<tr> 						
								<td >
									<p>Usuari</p>
								</td>
								<td>
									<input <?php echo "value=$usuari";?> type="text" name="usuari" class="espais"/> 
								</td>
							</tr>
							
							<tr> 
								<td >
									<p>Contrasenya</p>
								</td>
								<td>
									<input type="password" name="pass_1" class="espais"/> 
								</td>
							</tr>
							
							<tr> 
								<td>
									<p>Repetir Contrasenya</p>
								</td>
								<td>
									<input type="password" name="pass_2" class="espais"/> 
								</td>
							</tr>
							
							<tr>
								<td>
									<p>Nom i cognoms</p>
								</td>
								<td>
									<input <?php echo "value=$nom";?> name="nom_cognoms" class="espais"/>
								</td>
							</tr>

							<tr>
								<td>
									<p>Pes (Kg)</p>
								</td>
								<td>
									<input <?php echo "value=$pes";?> type="text" name="pes" class="espais"/>
								</td>
							</tr>

							<tr>
								<td>
									<p>Altura (Cm)</p>
								</td>
								<td>
									<input <?php echo "value=$altura";?> type="text" name="altura" class="espais"/>
								</td>
							</tr>

							<tr>
								<td>
									<p>Edat</p>
								</td>
								<td>
									<input <?php echo "value=$edat";?> type="text" name="edat" class="espais"/>
								</td>
							</tr>

							<tr>
								<td>
									<p>IMC</p>
								</td>
								<td>
									<input <?php echo "value=$imc";?> disabled/> 
								</td>
							</tr>

							<tr>
								<td>
									<input type="submit" id="boto" name="canviar" value="Canviar"/>
								</td>
							</tr>
						</form>
					</table>
				</td>
			</tr>
			
			<!-- LOGOUT -->
			<tr>
				<td class="imparell">
					<form method="POST" action="private/logout.php">
						<input id="logout" type="submit" value="Logout"/>
					</form>
				</td>
			</tr>
		</table>
		
		<!-- CONTROLADOR - PERFIL -->
		<?php
			include_once 'private/db.php';

			// Guardem les dades
			$user_antic = $_SESSION['usuari'];
			$canvi_user = mysqli_real_escape_string($conn, $_POST['usuari']);
			$canvi_nom = mysqli_real_escape_string($conn, $_POST['nom_cognoms']);
			$canvi_pes = mysqli_real_escape_string($conn, $_POST['pes']);
			$canvi_altura = mysqli_real_escape_string($conn, $_POST['altura']);
			$canvi_edat = mysqli_real_escape_string($conn, $_POST['edat']);

			$contrasenya_1 = mysqli_real_escape_string($conn, $_POST['pass_1']);
			$contrasenya_2 = mysqli_real_escape_string($conn, $_POST['pass_2']);

			// Actualitzem les dades en la BDD
			if ( isset($_REQUEST['canviar']) ) {
				$sql = "UPDATE usuaris SET usuari='" . $canvi_user ."',
											nom='" . $canvi_nom ."',
											pes='" . $canvi_pes . "',
											altura='" . $canvi_altura . "',
											edat='" . $canvi_edat . "'
											WHERE id = " . $_SESSION['id_usuari'];

				if (!$conn->query($sql) === TRUE) {
					echo "Error: " . $sql . "<br>" . $conn->error;
				}

				// Si elegeix canviar la contrasenya
				if ( !is_null($contrasenya_1) ) {
					if ($contrasenya_1 == $contrasenya_2 ){

						$sql = "UPDATE usuaris SET password='" . $contrasenya_1 ."' WHERE id = " . $_SESSION['id_usuari'];

						if (!$conn->query($sql) === TRUE) {
							echo "Error: " . $sql . "<br>" . $conn->error;
						}

						// Refresc per veure els canvis
						header('Location: perfil.php');	

					} else {
						echo "<script> alert('Les contrasenyes no coincideixen'); </script>";
					}
				} else {
					// Refresc per veure els canvis
					header('Location: perfil.php');		
				}					
			}				
		?>
		
	</body>
	
</html>
