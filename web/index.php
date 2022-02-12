<?php
require_once "vendor/autoload.php";

$mail_address = $_ENV['EMAIL_ADDRESS'];
$mail_password = $_ENV['EMAIL_PASSWORD'];

$mailer = new \PHPMailer\PHPMailer\PHPMailer(true);
$mailer->CharSet = "iso-2022-jp-ms";
$mailer->Encoding = "7bit";
$mailer->IsSMTP();
$mailer->Host = "smtp.gmail.com";
$mailer->SMTPAuth = true;
$mailer->SMTPDebug = 2;
$mailer->SMTPSecure = "tls";
$mailer->Port = 587;
$mailer->Username = $mail_address;
$mailer->Password = $mail_password;
$mailer->setFrom($mail_address, '冷蔵庫アラートくん');

$mailer->AddAddress($mail_address);
$mailer->Subject = mb_encode_mimeheader("冷蔵庫が開いてしまっています!", 'iso-2022-jp-ms');
$mailer->Body = mb_convert_encoding("冷蔵庫が開いてしまっています!", "iso-2022-jp-ms", "utf-8" );

$mailer->Send();
