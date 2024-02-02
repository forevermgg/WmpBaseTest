
#ifndef FOREVER_CONNECTION_TYPE_H_
#define FOREVER_CONNECTION_TYPE_H_

namespace FOREVER {
enum class ConnectionType : int {
        kUnknown = 0,  // A connection exists, but its type is unknown.
        // Also used as a default value.
        kEthernet = 1,
        kWifi = 2,
        k2G = 3,
        k3G = 4,
        k4G = 5,
        kNone = 6,  // No connection.
        kBluetooth = 7,
        k5G = 8,
        kLast = 9
};
}  // namespace FOREVER

#endif  // FOREVER_CONNECTION_TYPE_H_
