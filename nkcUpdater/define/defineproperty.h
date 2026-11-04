#ifndef QUX_DEFINEPROPERTY_H
#define QUX_DEFINEPROPERTY_H

#define PROPERTY(TYPE, NAME, SETTER)                                                                                                                    \
	Q_PROPERTY(TYPE NAME READ NAME WRITE SETTER NOTIFY NAME##Changed)                                                                                   \
	public:                                                                                                                                             \
		TYPE const& NAME() const { return m_##NAME; }                                                                                                   \
		void SETTER(TYPE const& NAME) { if (m_##NAME == NAME) return; emit NAME##AboutToBeChanged(NAME); m_##NAME = NAME; emit NAME##Changed(NAME); }   \
		Q_SIGNAL void NAME##AboutToBeChanged(TYPE const& NAME);                                                                                         \
		Q_SIGNAL void NAME##Changed(TYPE const& NAME);                                                                                                  \
	private:                                                                                                                                            \
		TYPE m_##NAME;

#define PROPERTY_NOCOMP(TYPE, NAME, SETTER)                                                                             \
	Q_PROPERTY(TYPE NAME READ NAME WRITE SETTER NOTIFY NAME##Changed)                                                   \
	public:                                                                                                             \
		TYPE const& NAME() const { return m_##NAME; }                                                                   \
		void SETTER(TYPE const& NAME) { emit NAME##AboutToBeChanged(NAME); m_##NAME = NAME; emit NAME##Changed(NAME); } \
		Q_SIGNAL void NAME##AboutToBeChanged(TYPE const& NAME);                                                         \
		Q_SIGNAL void NAME##Changed(TYPE const& NAME);                                                                  \
	private:                                                                                                            \
		TYPE m_##NAME;

#define PROPERTY_READONLY(TYPE, NAME, SETTER)                                                                                                           \
	Q_PROPERTY(TYPE NAME READ NAME WRITE SETTER NOTIFY NAME##Changed)                                                                                   \
	public:                                                                                                                                             \
		TYPE const& NAME() const { return m_##NAME; }                                                                                                   \
		Q_SIGNAL void NAME##AboutToBeChanged(TYPE const& NAME);                                                                                         \
		Q_SIGNAL void NAME##Changed(TYPE const& NAME);                                                                                                  \
	private:                                                                                                                                            \
		void SETTER(TYPE const& NAME) { if (m_##NAME == NAME) return; emit NAME##AboutToBeChanged(NAME); m_##NAME = NAME; emit NAME##Changed(NAME); }   \
		TYPE m_##NAME;

#define PROPERTY_READONLY_NOCOMP(TYPE, NAME, SETTER)                                                                    \
	Q_PROPERTY(TYPE NAME READ NAME WRITE SETTER NOTIFY NAME##Changed)                                                   \
	public:                                                                                                             \
		TYPE const& NAME() const { return m_##NAME; }                                                                   \
		Q_SIGNAL void NAME##AboutToBeChanged(TYPE const& NAME);                                                         \
		Q_SIGNAL void NAME##Changed(TYPE const& NAME);                                                                  \
	private:                                                                                                            \
		void SETTER(TYPE const& NAME) { emit NAME##AboutToBeChanged(NAME); m_##NAME = NAME; emit NAME##Changed(NAME); } \
		TYPE m_##NAME;

#define PROPERTY_NOCHANGE(TYPE, NAME)                   \
	Q_PROPERTY(TYPE NAME READ NAME)                     \
	public:                                             \
		TYPE const& NAME() const { return m_##NAME; }   \
	private:                                            \
		TYPE m_##NAME;

#define GETTER(TYPE, NAME)                              \
	public:                                             \
		TYPE const& NAME() const { return m_##NAME; }   \
	private:                                            \
		TYPE m_##NAME;

#endif