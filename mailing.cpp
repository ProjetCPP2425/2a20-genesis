#include "mailing.h"
#include <QProcess>

bool Mailing::sendMail(const QString &receiverEmail, const QString &subject, const QString &body) {
    QString pythonScript =
        "import smtplib\n"
        "from email.mime.text import MIMEText\n"
        "msg = MIMEText('" + body + "')\n"
                 "msg['Subject'] = '" + subject + "'\n"
                    "msg['From'] = 'your_email@gmail.com'\n"
                    "msg['To'] = '" + receiverEmail + "'\n"
                          "with smtplib.SMTP_SSL('smtp.gmail.com', 465) as server:\n"
                          "    server.login('youssefbahdoudi433@gmail.com', 'zugy kwgi dmqo gdic')\n"
                          "    server.send_message(msg)\n";

    QProcess process;
    process.start("python", QStringList() << "-c" << pythonScript);
    process.waitForFinished();

    return process.exitStatus() == QProcess::NormalExit;
}
