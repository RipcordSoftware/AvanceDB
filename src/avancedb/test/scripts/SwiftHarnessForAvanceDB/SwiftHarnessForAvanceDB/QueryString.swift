
import Foundation



/**
 * Query string
 */
public class QueryString {
    
    
    
    public init() {
        
    }
    
    
    
    /**
     * encode
     */
    public func encode() -> String {
        
        let mirror = Mirror(reflecting: self)
        var params = [String]()
        
        for (label, value) in mirror.children {
            
            if value.dynamicType is String.Type {
                params.append(handleString(label!, value: value))
            }

            else if value.dynamicType is Bool.Type {
                params.append("\(label!)=\(value)")
            }
            
            else if value.dynamicType is Int.Type {
                params.append(handleInt(label!, value: value))
            }
            
            else if value.dynamicType is [String].Type {
                if let arr = value as? [String] {
                    let vals = arr.map() {"\"\($0)\""}
                    let values = vals.joinWithSeparator(",")
                    params.append("\(label!)=[\(values)]")
                }
            }
            
            else {
                // handle optionals
                let sub = Mirror(reflecting: value)
                
                switch sub.displayStyle! {
                case .Optional:
                    
                    for (subLabel, subValue) in sub.children {
                        
                        if subLabel == "Some" {
                            
                            if subValue.dynamicType is String.Type {
                                params.append(handleString(label!, value: subValue))
                            }
                                
                            else if subValue.dynamicType is Int.Type {
                                params.append(handleInt(label!, value: subValue))
                            }
                            
                            else if subValue.dynamicType is Bool.Type {
                                params.append("\(label!)=\(subValue)")
                            }
                        }
                        
                    }
                    
                default:
                    break
                }
                
            }
        }
        
        return params.joinWithSeparator("&")
        
    }
    
    // handle String
    private func handleString(key: String, value: Any) -> String {
        return "\(key)=\"\(value)\""
    }
    
    private func handleInt(key: String, value: Any) -> String {
        return "\(key)=\(value)"
    }
    
}