#!/bin/bash
# Script: monitor_limits.sh
# Descripcion: 
#   Monitorea un proceso especifico y lo termina automaticamente si su uso
#   de CPU o memoria RAM supera un limite establecido. Esto es util para 
#   prevenir que el algoritmo A* sature la memoria o CPU del computador y lo
#   vuelva inestable
# Uso: 
#   ./monitor_limits.sh [nombre_del_proceso]
# Ejemplo:
#   ./monitor_limits.sh main
#   ./monitor_limits.sh test_AStarSolver
# Configuracion
# Nombre del proceso a monitorear. Se puede pasar como primer argumento
PROCESS_NAME="${1:-main}"  
# Limites de seguridad (en porcentaje 0.0 a 100.0)
# Nota: Un 100% de CPU usualmente significa un nucleo del procesador al maximo
CPU_THRESHOLD=50.0         
RAM_THRESHOLD=50.0         

# Intervalo de revision (en segundos)
CHECK_INTERVAL=2           

echo " Iniciando monitor de recursos"
echo " Proceso a monitorear : $PROCESS_NAME"
echo " Limite maximo de CPU : $CPU_THRESHOLD %"
echo " Limite maximo de RAM : $RAM_THRESHOLD %"
echo " Intervalo de revision: $CHECK_INTERVAL segundos"
echo " Presiona [Ctrl+C] para detener el monitor"

# Bucle infinito para monitorear periodiacamente
while true; do
    # Buscar el ID del proceso (PID) por su nombre
    # Usamos pgrep para encontrar todos los PIDs asociados al proceso
    PIDS=$(pgrep "$PROCESS_NAME")

    # Si el proceso no esta en ejecucion, esperamos y volvemos a intentar
    if [ -z "$PIDS" ]; then
        sleep "$CHECK_INTERVAL"
        continue
    fi

    # Si hay multiples instancias del proceso, revisamos cada una
    for PID in $PIDS; do
        # Obtener el uso actual de CPU y RAM usando el comando 'ps'
        STATS=$(ps -p "$PID" -o %cpu,%mem --no-headers)
        
        # Si se pudo obtener las estadisticas (el proceso aun existe)
        if [ -n "$STATS" ]; then
            # Leer los valores de CPU y Memoria (RAM)
            read -r CPU_USAGE MEM_USAGE <<< "$STATS"

            # Utilizar 'awk' para comparar numeros decimales, ya que bash
            # nativamente solo soporta operaciones con numeros enteros
            KILL_PROCESS=$(awk -v cpu="$CPU_USAGE" -v mem="$MEM_USAGE" \
                               -v max_cpu="$CPU_THRESHOLD" -v max_mem="$RAM_THRESHOLD" '
                BEGIN {
                    if (cpu > max_cpu || mem > max_mem) {
                        print 1
                    } else {
                        print 0
                    }
                }
            ')

            # Si se superan los limites, procedemos a matar el proceso
            if [ "$KILL_PROCESS" -eq 1 ]; then
                echo ""
                echo "[$(date +'%Y-%m-%d %H:%M:%S')] ¡ALERTA! Limites superados."
                echo "-> Proceso : $PROCESS_NAME (PID: $PID)"
                echo "-> CPU     : ${CPU_USAGE}% (Límite: $CPU_THRESHOLD%)"
                echo "-> RAM     : ${MEM_USAGE}% (Límite: $RAM_THRESHOLD%)"
                echo "Ejecutando 'kill -9' para detener el proceso y evitar bloqueos"
                
                # Forzar la terminacion del proceso de manera segura para el sistema
                kill -9 "$PID"
                
                echo "El proceso ha sido terminado exitosamente"
                echo "Esperando a que inicie nuevamente"
            fi
        fi
    done

    # Esperar antes de la siguiente revision
    sleep "$CHECK_INTERVAL"
done
