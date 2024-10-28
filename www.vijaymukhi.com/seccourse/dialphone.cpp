#include <eikapp.h>
#include <eikdoc.h>
#include <eikappui.h>
#include <eikbctrl.h>
#include <eikenv.h>
#include <e32std.h>
#include <ckninfo.h>
#include <stringloader.h>
#include <cknenv.h>
#include <Etel3rdParty.h>
#include <libc\stdio.h>
static const TUid KUidHelloWorldBasicApp = {0x10005B91};
class zCEikApplication : public CEikApplication
{
public:
TUid AppDllUid() const;
CApaDocument* CreateDocumentL();
};
class zCEikDocument : public CEikDocument
{
public:
CEikAppUi* CreateAppUiL();
zCEikDocument( CEikApplication& aApp );
};
class zCEikBorderedControl : public CEikBorderedControl
    {
    public:
        void Draw( const TRect& aRect ) const;
        zCEikBorderedControl(const TRect& aRect);
    };
class zCEikAppUi : public CEikAppUi
    {
    public:
        void ConstructL();
        zCEikAppUi();
        void HandleCommandL( TInt aCommand );
void DialNumberL( const TDesC& aPhoneNumber );

    };
TUid zCEikApplication::AppDllUid() const
{
return KUidHelloWorldBasicApp;
}
void zCEikAppUi::ConstructL()
{
BaseConstructL();
new zCEikBorderedControl( ClientRect() );
}
void zCEikAppUi::DialNumberL( const TDesC& aPhoneNumber )
{
//_LIT( aa ,"hi");
//#define _LIT(name,s) static const TLitC<sizeof(L##s)/2> name={sizeof(L##s)/2-1,L##s}
//_LIT16(KFormat1,"[%b %c %d %o %u %x]");
//tgt.Format(KFormat1,65,65,65,65,65,65);
//tgt.Format(_L("[%b %c %d %o %u %x]"),65,65,65,65,65,65);
_LIT(KTsyName,"phonetsy.tsy" );
TBuf <256> aa;
RTelServer server;
TInt e,e1;
e = server.Connect();
e1 = server.LoadPhoneModule(_L("phonetsy.tsy"));
TInt numberPhones;
server.EnumeratePhones(numberPhones);
aa.Format(_L("Connect returns %d LoadPhonModule returns %d Number of phones %d"),e,e1,numberPhones);
//CCknInfoDialog::RunDlgLD(_L("hi"),aa);
RTelServer::TPhoneInfo info;
server.GetPhoneInfo(0,info);
RPhone phone;
phone.Open(server,info.iName);
TInt i;
phone.EnumerateLines(i);
aa.Format(_L("Phone Name %S number of lines %d"),&info.iName,i);
//CCknInfoDialog::RunDlgLD(_L("hi"),aa);
RPhone::TLineInfo lineInfo;
TInt j;
/*
for (j = 0 ; j < i ; j++)
{
phone.GetLineInfo( j, lineInfo );
CCknInfoDialog::RunDlgLD(_L("hi"),lineInfo.iName);
}
*/
phone.GetLineInfo( 0, lineInfo );
RLine line;
line.Open( phone, lineInfo.iName ) ;
TBuf <100> newCallName;
RCall call;
call.OpenNewCall( line, newCallName );
//CCknInfoDialog::RunDlgLD(_L("hi"),newCallName);

call.Dial(aPhoneNumber );


server.UnloadPhoneModule(KTsyName);
}

void zCEikAppUi::HandleCommandL( TInt aCommand )
{
if ( aCommand == EEikCmdExit)
CBaActiveScheduler::Exit();
if ( aCommand == 1)
{
_LIT(KPhoneNumber,"9821896396");
DialNumberL( KPhoneNumber );
}
}
zCEikBorderedControl::zCEikBorderedControl(const TRect& aRect)
{
CreateWindowL();
SetRect( aRect );
ActivateL();
}
void zCEikBorderedControl::Draw( const TRect& aRect ) const
{
CWindowGc& gc = SystemGc();
gc.UseFont( iCoeEnv->NormalFont() );
_LIT( aa,"Information339" );
gc.DrawText( aa, TPoint( 30,130 ) );
}
GLDEF_C TInt E32Dll( TDllReason /*aReason*/ )
{
return KErrNone;
}
EXPORT_C CApaApplication* NewApplication() 
{
return ( new zCEikApplication );
}
CApaDocument* zCEikApplication::CreateDocumentL()
{  
return (new zCEikDocument(*this));
}
zCEikDocument::zCEikDocument( CEikApplication& aApp ) : CEikDocument( aApp ) 
{
}
CEikAppUi* zCEikDocument::CreateAppUiL()
{
return (new zCEikAppUi);
}
zCEikAppUi::zCEikAppUi()                              
{
}
