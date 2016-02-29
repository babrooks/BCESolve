#ifndef BCELOGSTREAM_HPP
#define BCELOGSTREAM_HPP

#include <iostream>
#include <streambuf>
#include <string>
#include <QTextEdit>

//! Redirects all couts in the program to the log tab.
/*! This class, initialized in main, redirects all couts
  to the QTextEdit in the log tab. This class contains
  QMetaObject calls in order to redirect couts from 
  other threads.

  \ingroup viewer

 */
class BCELogStream : public std::basic_streambuf<char>
{
public:
  //! Constructor
  BCELogStream(std::ostream &stream, QTextEdit* text_edit) : m_stream(stream)
  {
    log_window = text_edit;
    m_old_buf = stream.rdbuf();
    stream.rdbuf(this);
  }

  //! Destructor
  ~BCELogStream()
  {
    // output anything that is left
    if (!m_string.empty()) {
      QMetaObject::invokeMethod(log_window, "append", Qt::QueuedConnection, 
				Q_ARG(QString, m_string.c_str()));
      // log_window->append(m_string.c_str());
    }

    m_stream.rdbuf(m_old_buf);
  }

protected:
  virtual int_type overflow(int_type v)
  {
    if (v == '\n')
      {
	QMetaObject::invokeMethod(log_window, "append", Qt::QueuedConnection, 
				  Q_ARG(QString, m_string.c_str()));
	// log_window->append(m_string.c_str());
	m_string.erase(m_string.begin(), m_string.end());
      }
    else
      m_string += v;

    return v;
  }

  virtual std::streamsize xsputn(const char *p, std::streamsize n)
  {
    m_string.append(p, p + n);

    int pos = 0;
    while (pos != std::string::npos)
      {
	pos = m_string.find('\n');
	if (pos != std::string::npos)
	  {
	    std::string tmp(m_string.begin(), m_string.begin() + pos);
	    QMetaObject::invokeMethod(log_window, "append", Qt::QueuedConnection, 
				      Q_ARG(QString, tmp.c_str()));
	    // log_window->append(tmp.c_str());
	    m_string.erase(m_string.begin(), m_string.begin() + pos + 1);
	  }
      }

    return n;
  }

private:
  //! Text Edit where redirected stout/stderr will be sent
  QTextEdit* log_window;

  //! Type of ostream redirected (e.g. "cout," "cerr," etc.)
  std::ostream &m_stream;
  std::streambuf *m_old_buf;
  std::string m_string;


};

#endif
