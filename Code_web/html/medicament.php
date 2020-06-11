<html>
	
	<head>
		<link rel="stylesheet" type="text/css" href="css/style.css"/>
		<link rel="shortcut icon" href="img/pill.ico" type="image/x-icon"/>
		<title>Chemist Bot</title>
	</head>
	
	<body>
		<nav>
			<ul>
				<li><a href="medicament.php" class="actual">Medicaments</a></li>
				<li><a href="dispensar.php">Dispensar</a></li>
				<li><a href="horaris.php">Horaris programats</a></li>
				<li><a href="perfil.php">Perfil</a></li>
			</ul>
		</nav>

		<table width=100% height=92% cellspacing=0>
			<!-- VISTA - AFEGIR MEDICAMENT -->
			<tr>
				<td class="imparell" width = 33%>
				<table border = 0px width=100% >
					<tr>
						<td>
							<h2> Afegir nou medicament </h2>
						</td>
					</tr>

					<form action="#" method="post"> 
						<tr> 						
							<td>
								<input placeholder="Nom medicament" type="text" name="nom" class="espais"/> 
							</td>
						</tr>

						<tr>
							<td>
								<input placeholder="Quantitat" type="text" name="quantitat" class="espais"/>
							</td>
						</tr>

						<tr>						
							<td>
								<input placeholder="Numero cartutx" type="text" name="cartutx" class="espais"/>
							</td>
						</tr>

						<tr>
							<td>
								<input type="submit" id="boto" name="afegir" value="Afegir"/>
							</td>
						</tr>
					</form>
				</table>
				</td>
			</tr>
			
			<!-- VISTA - ESBORRAR MEDICAMENT -->
			<tr>
				<td class="parell" width = 33%>
				<table border = 0px width=100%> 
					<tr>
						<td>
							<h2> Esborrar medicament </h2>
						</td>
					</tr>
					
					<form action="#" method="get">
					<tr>
						<td>
							<select name="esborrat" size=5>
							<?php
								include_once 'private/db.php';
								$query = mysqli_query($conn, "SELECT * FROM medicaments");
								$row = $conn->query($query);
								$contador = $row->num_rows;

								// Imprimim tots els noms dels medicaments que hi ha
								if (mysqli_num_rows($query) > 0) {
									while($row = mysqli_fetch_array($query)) {
										echo "<option value = '". $row['id']."'> " . $row['nom'] . "</option>";	
									}
								} else {
									// Si no hi ha cap, imprimim un valor predeterminat
									echo "<option value ='none'>No hi ha cap medicament</option>";
								}
							?>
							</select> 
						</td>
					</tr>
						
					<tr>
						<td> 
							<input type="submit" id="boto2" name="esborrar" value="Esborrar"/>
						</td>
					</tr>
					</form>
					
				</table>
				</td>
			</tr>
				
			<!-- VISTA - REPOSTAR MEDICAMENT -->
			<tr>
				<td class="imparell" width = 33%>
				<table border = 0px width = 100%>
					<tr>
						<td>
							<h2> Repostar medicament </h2>
						</td>
					</tr>
					
					<form action="#" method="POST">
						<tr>
							<td>

								<select name="repostat" size="5">
								<?php
									include_once 'private/db.php';
									$query = mysqli_query($conn, "SELECT * FROM medicaments");
									$row = $conn->query($query);
									$contador = $row->num_rows;

									// Imprimim tots els medicaments que hi ha, la seva quantitat i en quin cartutx es troba
									if (mysqli_num_rows($query) > 0) {
										while($row = mysqli_fetch_array($query)) {
											echo "<option  value = '". $row['id'] . "' >".$row['nom'] . " | Quantitat: " 
												. $row['quantitat'] . " | cartutx: " . $row['cartutx'] . "</option>";	
										}
									} else {
										// Si no hi ha cap, imprimim un valor predeterminat
										echo "<option value ='none'>No hi ha cap medicament</option>";
									}
								?>
								</select> 
							</td>
						</tr>

						<tr>
							<td>
								<input placeholder="Nova quantitat" type="text" name="nova_quantitat"/>
							</td>
						</tr>
						
						<tr>
							<td> 
								<input type="submit" id="boto3" name="repostar" value="repostar"/>
							</td>	
						</tr>
					</form>
				</table>
				</td>
			</tr>
		</table>
				
		<!-- CONTROLADORS -->
		<?php
			include_once 'private/db.php';

			// Mirem si està logejat
			session_start();
		
			if ($_SESSION['estat'] == 0){
				header("Location:index.php");
			}
		
			// Variables
			$nom = mysqli_real_escape_string($conn, $_POST['nom']);
			$quantitat = mysqli_real_escape_string($conn, $_POST['quantitat']);
			$cartutx = mysqli_real_escape_string($conn, $_POST['cartutx']);
			$nova_quantitat = mysqli_real_escape_string($conn, $_POST['nova_quantitat']);
			$id = 0;
			
			// Mirem la ultima id per utilitzar la següent
			$query = mysqli_query($conn, "SELECT max(id) id FROM medicaments");
			while($row = mysqli_fetch_array($query)) {
				$id = $row["id"] + 1;
			}
		
			// CONTROLADOR - AFEGIR NOU MEDICAMENT
			if ( isset($_REQUEST['afegir']) ) {
				$sql = "INSERT INTO medicaments VALUES ($id, '$nom', $quantitat, $cartutx)";
				header('Location: medicament.php');	// Refresc per veure els canvis

				if (!$conn->query($sql) === TRUE) {
			  		echo "Error: " . $sql . "<br>" . $conn->error;
				}
			}

			// CONTROLADOR - ESBORRAR MEDICAMENT
			if ( isset($_REQUEST['esborrar']) ) {
				$sql = "DELETE FROM medicaments WHERE id = '" . $_GET['esborrat'] . "'";
				header('Location: medicament.php');	// Refresc per veure els canvis
				
				if (!$conn->query($sql) === TRUE) {
			  		echo "Error: " . $sql . "<br>" . $conn->error;
				}
			}
		
			// CONTROLADOR - REPOSTAR MEDICAMENT
			if ( isset($_REQUEST['repostar'])) { // && $_REQUEST['repostat'] > 0
				$sql = "UPDATE medicaments SET quantitat = " . $nova_quantitat . " WHERE id= ". $_REQUEST['repostat'];
				header('Location: medicament.php');	// Refresc per veure els canvis
				
				if (!$conn->query($sql) === TRUE) {
			  		echo "Error: " . $sql . "<br>" . $conn->error;
				}
			}
		?>
	</body>
</html>