
Imports System
Imports System.IO.Ports


Public Class Form1

    Dim comPORT As String
    Dim receivedData As String = ""
    Dim connected As Boolean = False
    Dim count = 0

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        populateCOMport()
    End Sub

    Private Sub populateCOMport()
        comPORT = ""
        comPort_ComboBox.Items.Clear()
        For Each sp As String In My.Computer.Ports.SerialPortNames
            comPort_ComboBox.Items.Add(sp)
        Next
    End Sub

    Private Sub refreshCOM_CB_BTN_Click(sender As Object, e As EventArgs) Handles refreshCOM_CB_BTN.Click
        If connected = False Then
            SerialPort1.Close()
            populateCOMport()
        End If
    End Sub


    Private Sub comPort_ComboBox_SelectedIndexChanged(sender As Object, e As EventArgs) Handles comPort_ComboBox.SelectedIndexChanged
        If (comPort_ComboBox.SelectedItem <> "") Then
            comPORT = comPort_ComboBox.SelectedItem
        End If
    End Sub

    Private Sub connect_BTN_Click(sender As Object, e As EventArgs) Handles connect_BTN.Click
        comPORT = comPort_ComboBox.SelectedItem
        If (connect_BTN.Text = "Connect") Then
            If (comPORT <> "") Then
                SerialPort1.Close()
                SerialPort1.PortName = comPORT
                SerialPort1.BaudRate = 9600
                SerialPort1.DataBits = 8
                SerialPort1.Parity = Parity.None
                SerialPort1.StopBits = StopBits.One
                SerialPort1.Handshake = Handshake.None
                SerialPort1.Encoding = System.Text.Encoding.Default
                SerialPort1.ReadTimeout = 10000

                SerialPort1.Open()

                'See if the Arduino is there
                count = 0
                SerialPort1.WriteLine("<PING>")
                connect_BTN.Text = "Connecting..."
                connecting_Timer.Enabled = True
            Else
                MsgBox("Select a COM port first")
            End If
        Else
            Timer1.Enabled = False
            Timer_LBL.Text = "Timer: OFF"
            SerialPort1.Close()
            connected = False
            connect_BTN.Text = "Connect"
            populateCOMport()
        End If

    End Sub



    Private Sub connecting_Timer_Tick(sender As Object, e As EventArgs) Handles connecting_Timer.Tick
        connecting_Timer.Enabled = False
        count = count + 1

        If (count <= 8) Then
            receivedData = receivedData & ReceiveSerialData()

            If (Microsoft.VisualBasic.Left(receivedData, 4) = "PING") Then
                connected = True
                connect_BTN.Text = "Disconnect"
                Timer1.Enabled = True
                Timer_LBL.Text = "Timer: ON"
                receivedData = ReceiveSerialData()
                receivedData = ""
                SerialPort1.WriteLine("<START>")

            Else
                connecting_Timer.Enabled = True
            End If






        Else
            'time out (8 * 250 = 2 seconds)
            RichTextBox1.Text &= vbCrLf & "ERROR" & vbCrLf & "Can not connect" & vbCrLf
            connect_BTN.Text = "Connect"
            populateCOMport()
        End If



    End Sub




    Private Sub Timer1_Tick(sender As Object, e As EventArgs) Handles Timer1.Tick
        receivedData = ReceiveSerialData()
        RichTextBox1.AppendText(receivedData)

        Dim IRValue As String
        IRValue = Microsoft.VisualBasic.Left(receivedData, 3)  'IR0 or IR1
        If (IRValue.Length = 3) Then
            If (String.Compare(IRValue.Substring(0, 2), "IR") = 0) Then
                IR_label.Text = IRValue.Substring(2, 1)

            End If
        End If

    End Sub



    Function ReceiveSerialData() As String
        Dim Incoming As String
        Try
            Incoming = SerialPort1.ReadExisting()
            If Incoming Is Nothing Then
                Return "nothing" & vbCrLf
            Else
                Return Incoming
            End If
        Catch ex As TimeoutException
            Return "Error: Serial Port read timed out."
        End Try

    End Function

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        If (connected) Then
            If (Button1.Text = "OPEN GATE") Then
                SerialPort1.WriteLine("<OPENGATE>")
                Button1.Text = "CLOSE GATE"
            Else
                SerialPort1.WriteLine("<CLOSEGATE>")
                Button1.Text = "OPEN GATE"
            End If
        Else
            MsgBox("Not connected")
        End If
    End Sub

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        RichTextBox1.Clear()
    End Sub
End Class
