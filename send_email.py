import smtplib
import sys

def send_email(to_email, subject, body):
    sender_email = "gestini.alerte@gmail.com"  # Remplacez par votre adresse email
    sender_password = "gestiniadmin"  # Assurez-vous d'utiliser un mot de passe d'application

    try:
        server = smtplib.SMTP("smtp.gmail.com", 587)  # Pour Gmail
        server.starttls()
        server.login(sender_email, sender_password)

        message = f"Subject: {subject}\n\n{body}"
        server.sendmail(sender_email, to_email, message)
        server.quit()

        print("Email envoyé avec succès !")

    except Exception as e:
        print(f"Erreur lors de l'envoi de l'email: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: send_email.py <email> <sujet> <message>")
        sys.exit(1)

    send_email(sys.argv[1], sys.argv[2], sys.argv[3])
