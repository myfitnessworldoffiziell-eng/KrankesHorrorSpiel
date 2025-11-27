#pragma once

#include <string>

/**
 * PermissionManager
 * Verwaltet Benutzer-Zustimmungen für:
 * - Kamera-Zugriff
 * - Mikrofon-Zugriff
 * - Datei-Erstellung außerhalb Spielordner
 *
 * WICHTIG: Alles transparent und mit expliziter Zustimmung!
 */
class PermissionManager {
public:
    PermissionManager();
    ~PermissionManager();

    // Kamera
    bool hasCameraPermission() const { return m_cameraAllowed; }
    void requestCameraPermission();
    void revokeCameraPermission() { m_cameraAllowed = false; }

    // Mikrofon
    bool hasMicrophonePermission() const { return m_microphoneAllowed; }
    void requestMicrophonePermission();
    void revokeMicrophonePermission() { m_microphoneAllowed = false; }

    // Datei-Zugriff
    bool hasFileCreationPermission() const { return m_fileCreationAllowed; }

private:
    bool showPermissionDialog(const std::string& title, const std::string& message);

    bool m_cameraAllowed;
    bool m_microphoneAllowed;
    bool m_fileCreationAllowed;
};
